# AI Unit-Demand Accounting and Production Saturation

## Baseline and objective

Implementation branch: `codex/ai-unit-demand-accounting`

Integrated base: `467f214175c228ed58772eda015f44f1593b42e6`

The objective is to reduce AI unit bloat by making the player, rather than each
city, authoritative for production demand. Migrated production decisions count
live units, queued units, and current-cycle production reservations before
advertising more work.

The implementation must preserve existing `AI_totalUnitAIs()` and area-total
semantics, existing serialized queues, multiplayer determinism, and NPC/barbarian
production. It must not prune existing queues or add an LLM/network dependency.

## Design decisions

- Add a typed, unsaved production-reservation table to `CvContractBroker`.
  Operational unit-assignment contracts remain separate.
- Expire uncommitted city-originated production requests at the next owner
  production cycle. Unit-originated operational requests persist while valid.
- Use cumulative economic quantity tiers. A hard emergency minimum does not
  promote the entire desired quantity to emergency status.
- Separate request-admission results from tender-fulfillment results.
- Commit a reservation only after queue insertion succeeds and training counters
  have incremented.
- Migrate explicit numerical production targets in bounded waves. Exact-city
  defense, healer, property-control, and other assignment-or-production requests
  are deferred.

## Types and interfaces

Add C++03-compatible, initialized demand enums and value structures for:

- Scope: player, land area, or water area.
- Selector: UnitAI role or exact unit.
- Class: emergency, essential, strategic, or optional.
- Policy: stable identity for each migrated supply pool.
- Admission and fulfillment reason codes.
- Cumulative desired targets, supply snapshots, demand keys, and request results.

The stable demand key is:

```text
owner + policy + selector + UnitAI/exact UnitType
+ scope + area ID + complete selection criteria
```

Source city, priority, and economic class are not identity fields. Selection
criteria use complete field-by-field comparison; the existing compact hash is
not authoritative.

`CvPlayerAI` exposes:

```cpp
AIUnitRoleSupply AI_getUnitRoleSupply(
    UnitAITypes eUnitAI,
    AIUnitDemandScopeTypes eScope,
    const CvArea* pArea
) const;

int AI_getUnitRoleDeficit(
    UnitAITypes eUnitAI,
    int iDesired,
    AIUnitDemandScopeTypes eScope,
    const CvArea* pArea
) const;

AIUnitDemandResult AI_requestUnitDemandIfNeeded(
    CvCityAI* pRequestingCity,
    const AIUnitDemandKey& kKey,
    const AIUnitDemandTarget& kTarget,
    int iPriority,
    int iMaxUnitSpendingPercent,
    const CvUnitSelectionCriteria* pCriteria
);
```

`CvCityAI::AI_chooseUnitForDemand(...)` preserves branch eligibility and RNG
placement, then performs authoritative reservation admission. Reservation
acceptance does not mean that the requesting city immediately receives an order;
tender allocation remains an end-of-turn broker responsibility.

## Reservation lifecycle

1. At `AI_doTurnPre()`, after counter reconciliation and before city production,
   begin a new demand cycle and discard uncommitted typed reservations from the
   prior cycle.
2. Repeated submissions for the same key merge component-wise by maximum
   cumulative target. A later city cannot lower the aggregate based on iteration
   order.
3. Reserve only:

   ```text
   max(0, economically allowed desired - existing - training)
   ```

4. Index pending quantities immediately by role, policy, scope, and area.
5. Pure production reservations are not offered to existing units through
   `makeContract()` because live units are already part of supply.
6. Tender finalization processes aggregate quantities one at a time in priority
   descending, stable request-ID, then quantity-ordinal order.
7. Successful queue insertion converts pending to training without changing
   effective supply.
8. No-producer, invalid-target, no-path, economic-recheck, and queue-rejection
   outcomes are logged and never make a transient request immortal.

Existing `workRequest` records gain explicit origin/kind, creation/refresh turns,
source city where applicable, and expiration policy. City-originated legacy
production requests require next-cycle refresh; unit-originated operational
requests remain persistent while valid.

## Economic saturation

Cache once per AI turn:

- Strike state.
- Critical-gold state.
- Financial-trouble state.
- Current unit-upkeep percentage.

The state precedence is:

```text
STRIKE
CRITICAL_GOLD
FINANCIAL_TROUBLE
UNIT_SATURATED
NORMAL
```

`UNIT_SATURATED` means current unit-upkeep percentage has reached the demand
policy's permitted unit-spending threshold. Repeated requesters retain the
maximum valid threshold.

| Class | Normal | Saturated | Financial trouble | Critical gold | Strike |
|---|---:|---:|---:|---:|---:|
| Emergency | Allow | Allow | Allow | Allow | Allow |
| Essential | Allow | Allow | Allow | Reject | Reject |
| Strategic | Allow | Reject | Reject | Reject | Reject |
| Optional | Allow | Reject | Reject | Reject | Reject |

Targets are cumulative. Worker demand, for example, has emergency desired supply
of one and essential desired supply of `AI_neededWorkers(area)`, so only the hard
first worker bypasses critical-gold and strike gates.

Tender finalization recomputes the gate before every commitment. Random and
least-represented fallback production stays outside demand arithmetic but must
pass a strategic or optional economic gate.

## Counter integrity and performance

Extend `AI_recalculateUnitCounts()` to rebuild:

- Player live UnitAI counts from units.
- Player training UnitAI counts from queued train orders.
- Area live counts from unit locations.
- Area training counts from producing-city area association.
- Water-area supplemental counts matching existing semantics.

An unsaved water-area cache includes units owned by the queried player standing
on coastal city plots attached to that water area, regardless of city owner, and
the player's queued units in owned coastal cities attached to that water area.
Build it at a safe player boundary and update it at queue and movement mutation
points; invalidate it for safe-boundary rebuilding when city ownership or
destruction makes incremental repair uncertain.

Debug reconciliation independently scans units, queues, areas, water
supplements, and broker records once per player turn. It verifies nonnegative
counts, live/training agreement, broker-index agreement, bounded pending
quantity, and exact lifecycle deltas for completion, cancellation, upgrade,
role/area/ownership change, death, capture, and city loss.

Release builds log impossible mutations, set the existing recalculation flag,
and rebuild at the next safe boundary. They do not silently clamp corruption.

Player/land/water supply lookup must be cached; reservation upsert is logarithmic;
full scans are debug-only, load-time, or repair-triggered.

## Migration waves

Each wave is separately reviewable behind a temporary compile-time guard. The
next wave starts only after build, reconciliation, targeted autoplay, and
save/reload validation of the prior wave.

### Wave 1

- Settlers: strategic, remove the manual outstanding-request addition.
- Workers: land-area, emergency hard minimum one, remaining target essential.
- Sea workers: water-area essential, preserving the map-size cap.
- Hunters: land-area essential minimum one and optional remainder, preserving
  the huge-map cap.
- Land/sea explorers: area-scoped optional.
- Spies and infiltrators: land-area strategic.

Repeated formulas for one role/scope use one policy identifier and one
authoritative target.

### Wave 2

- Missionaries/executives use exact selected unit or explicit spread capability.
- Explicit attack, attack-city, and collateral targets.
- Naval transport, escort, attack-sea, carrier, missile-carrier, and pirate
  targets.
- Carrier aircraft, missile aircraft, other explicit aircraft, and nuclear
  targets.
- Sea spy, missionary, settler, and explorer targets with explicit needs.

Pending production is never included in danger, combat strength, attack odds,
stack availability, bombard composition, or tactical force assessment.

### Deferred

Exact-city defenders, property responders, stack/city healers, and other needs
that existing units may satisfy through assignment remain on the legacy path in
this series. Their city-origin requests receive one-cycle expiration, but their
target/capability semantics require a separate follow-up.

## Save, multiplayer, logging, and rollback

- Demand records, indexes, economic snapshots, and water caches remain unsaved.
- Reconcile queues and rebuild caches before the first post-load AI production
  cycle.
- Keep new enums out of serialized/XML-remapped global enum numbering.
- Use ordered containers and explicit stable state-changing iteration.
- Authoritative pending supply may intentionally remove RNG draws that redundant
  cities previously made, but identical clients must remain deterministic.
- Emit structured admission, fulfillment, and reconciliation logs only when the
  relevant logging level is enabled.
- Do not prune existing queues.
- Call-site migration commits can be reverted independently while retaining
  reconciliation and diagnostics.

## Acceptance

Debug and Release builds must pass. Validation covers:

1. Multi-city reservation never exceeds deficit.
2. Existing worker, sea-worker, hunter, settler, and explorer caps remain.
3. Economic tiers admit only their permitted cumulative quantity.
4. Pending converts to training only after accepted queue insertion.
5. Failed/no-producer/invalid requests do not persist beyond the cycle.
6. All unit/queue/area/ownership lifecycle transitions reconcile.
7. Legacy operational contracts and NPC/barbarian production remain functional.
8. Identical-save autoplay produces identical demand/queue checksums.
9. Large-map autoplay shows bounded broker growth and no migrated-role streak
   after target satisfaction.
10. Three comparable 20-turn large-map windows after a five-turn warm-up regress
    median AI turn time by no more than five percent with logging disabled.

Record desired targets, economy, existing/training/pending/effective supply,
admission/fulfillment results, queue mutation, broker size, and reconciliation
for every scenario.

The MVP is complete when Wave 1 and Wave 2 explicit numeric targets use
authoritative supply, transient city production requests cannot persist without
refresh, failed insertion cannot consume demand, counters reconcile, saves remain
compatible, multiplayer stays deterministic and offline, NPC behavior is
unchanged, and the performance/broker-growth gates pass.

Full local-support migration and any LLM policy adapter are explicitly outside
this MVP.
