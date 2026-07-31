# AI Unit-Demand Accounting Wave 1 Verification

## Status

Partial runtime pass. The first 65-turn playtest validated production-demand
admission, economic gating, checked queue commitment, and bounded legacy broker
growth. It also exposed an initialization defect in the unsaved water-area
supplemental cache. A fresh-start retest reduced the resulting discrepancies
from 16 to two and isolated the remaining first-city-founding transition. Both
cache defects have now been repaired and the second fresh-start test passed
their reconciliation gate. That test exposed a separate tender revalidation
defect that could expand an economically gated reservation after admission.
The tender defect is repaired and rebuilt, but its latest DLL still requires a
focused confirmation and save/reload retest before Wave 2.

Wave 2 remains gated.

## Test identity

- Test save: `UNIT_SPAM_FIX_TEST.CivBeyondSwordSave`
- Fresh-start retest save:
  `UNIT_SPAM_FIX_TEST_FRESH_RETEST.CivBeyondSwordSave`
- Second fresh-start retest save:
  `UNIT_SPAM_FIX_TEST_FRESH_RETEST_2.CivBeyondSwordSave`
- Save path: `My Games\Beyond The Sword\Saves\single`
- Approximate turns played: 65
- Structured demand records: 86, covering turns 0 through 62
- AI players represented in demand records: 7
- Tested branch before the water-cache repair:
  `codex/ai-unit-demand-accounting` at `410fbe296`
- User-observed result: early production mixed units with important first
  structures; Wanderers remained numerous but plausible; the player's Wanderer
  survived repeated proximity to big cats and exploration felt better.

Codex did not launch or advance the game. All quantitative findings below came
from the save and logs produced by the user-run session.

## Demand-accounting results

The 86 `AI_UNIT_DEMAND` records produced:

- Zero `existing + training + pending != effective` mismatches.
- Zero negative structured demand values.
- Zero cases where pending production exceeded
  `max(0, desired - existing - training)`.
- Zero remaining-deficit mismatches.
- Zero accepted requests with no corresponding pending quantity.

Admission outcomes:

| Outcome | Records |
|---|---:|
| Accepted | 20 |
| Refreshed | 10 |
| No deficit | 5 |
| Partially economically gated | 10 |
| Fully economically rejected | 41 |

The partial and full gates suppressed 230 repeated requested quantities during
the observed evaluation opportunities. Economy states represented in the
records were normal, unit-saturated, and financial-trouble. Critical-gold and
strike behavior were not exercised.

Policies exercised:

| Policy | Records | Maximum desired | Maximum pending |
|---|---:|---:|---:|
| Worker | 1 | 1 | 0 |
| Hunter | 53 | 6 | 6 |
| Land explorer | 6 | 5 | 5 |
| Sea explorer | 6 | 5 | 0 |
| Spy | 3 | 2 | 0 |
| Infiltrator | 17 | 1 | 1 |

Settler and sea-worker demand did not produce structured records during this
short prehistoric window.

## Tender and queue results

- Successful typed commitments: 30.
- No-eligible-tender outcomes: 8.
- Checked queue rejections: 10.
- Other fulfillment failures: 0.

Every checked queue rejection followed two successful commitments for the same
request and city in the same tender pass. This is consistent with the existing
same-unit/queue cap preventing a third insertion. The rejected quantity remained
uncommitted as designed.

Legacy broker work-request counts were sampled 910 times:

- Maximum active legacy requests: 3.
- Mean active legacy requests: 0.12.
- No invalid join-unit records appeared after the plot-contract assertion fix.

These results show bounded broker growth in this test, but they do not yet
exercise a mature multi-city empire coordinating one same-scope demand.

## Production mix

The BBAI log recorded 29 completed AI units:

| Completed role | Count |
|---|---:|
| Hunter | 16 |
| Hunter escort | 5 |
| Attack | 4 |
| Explorer | 4 |

Completed concrete units were 16 Brutes and 13 Wanderers. The Wanderers were
assigned as:

| Wanderer role | Count |
|---|---:|
| Hunter | 6 |
| Hunter escort | 4 |
| Explorer | 3 |

Therefore, the visible Wanderer volume was mostly the prehistoric unit selected
for hunter and escort demand, not repeated generic explorer demand.

The log also recorded 60 building queue events, including Alpha Male, Alpha
Female, gatherers, Knowledge Inheritance, Community Discussions, and folklore
buildings. This supports the user's observation that infrastructure remained
mixed into production. The current BBAI logging does not provide a symmetric
building-completion record, so these are queue events rather than claimed
completion totals.

## Water-cache defects found and repaired

The BBAI log contained 16 `AI_UNIT_RECONCILE` records for negative
`water-live` supplemental counts. They occurred when starting units moved from
or died on coastal city plots before the unsaved cache had received its first
authoritative safe-boundary rebuild.

Repair:

- Added an explicit unsaved water-cache validity state.
- Incremental live/training changes are ignored while the cache is invalid.
- The safe player boundary builds the cache from units and city queues and marks
  it valid.
- Impossible mutations after initialization invalidate the cache, log the
  discrepancy, and schedule primary-object reconciliation without storing a
  negative count.
- Existing uncertain mutations such as city loss now invalidate the water cache
  through the established recalculation flag.

The repair passes Debug and Release DLL builds. The corrected Debug DLL was
deployed after the original test; no game was launched by Codex.

- Repair commit: `bf6f5fa9f`
- Corrected Debug DLL SHA-256:
  `64288191E71645EC29E8478C0E247891F6689D765353585B904EB0709844959F`
- Corrected Release DLL SHA-256:
  `1436C100C71B1AA1942A76B03F9B8014DF14835603F7038CE93741A927CABBC9`

The user then ran `UNIT_SPAM_FIX_TEST_FRESH_RETEST`. Its logs contained:

- Zero assertion markers.
- Zero Python errors.
- Two `AI_UNIT_RECONCILE` records, down from 16.
- Both discrepancies were `water-live` decrements for `UNITAI_SETTLE`, one
  each for players 6 and 7 immediately after founding their first city.

The safe-boundary cache correctly began valid and empty because neither
settler was standing on a city plot yet. Founding its first coastal city changed
the plot into a coastal-city plot without moving the settler through
`CvUnit::setXY()`. The settler's later death therefore attempted to remove a
supplemental count that city founding had never added.

Second repair:

- When a coastal city is initialized, units already standing on its plot are
  added to their respective owners' valid water-area supplemental caches.
- Temporary units and units without a valid `UnitAI` role remain excluded,
  matching full-rebuild semantics.
- Invalid caches still ignore incremental changes and rebuild at the next safe
  boundary.
- City loss continues to invalidate the cache for safe reconstruction.

The second repair passes Debug and Release DLL builds and its Debug DLL is
deployed. Codex did not launch or advance the game.

- Second repair commit: `4795a4b64`
- Latest Debug DLL SHA-256:
  `B0408A603CF372F8641930EA2D82051E4FBA44445C1D693471ED8F7906A2C71F`
- Latest Release DLL SHA-256:
  `8C6151E19E314E23E470BD1C1B05CB4E99440D8A6817A90109A5010CFC963977`

## Second fresh-start result and tender repair

`UNIT_SPAM_FIX_TEST_FRESH_RETEST_2` covered demand turns 0 through 2 with
the second water-cache repair. Its logs contained:

- Zero assertions or fatal-error markers.
- Zero Python errors.
- Zero `AI_UNIT_RECONCILE` records.
- Sixteen demand-admission records and nine fulfillment records.
- Zero negative demand values.
- Zero effective-supply, deficit, or pending-ceiling invariant failures.

This clears the water-cache first-city-founding confirmation.

The same log exposed one separate economic-tier violation. An AI hunter policy
submitted a cumulative desired target of eleven while in financial trouble.
Admission correctly reserved one essential hunter and gated the other ten, but
tender finalization refreshed the economic state and expanded the outstanding
reservation back to eleven. It committed two units before the existing queue
cap rejected a third.

This was not a counter discrepancy: it was an unauthorized increase between
admission and fulfillment. It explains why some earlier queue rejections
followed two commitments even when admission had allowed only one.

Tender repair:

- Revalidation may reduce an admitted reservation when the economy worsens or
  supply increases.
- Revalidation may never increase the reservation beyond the quantity actually
  admitted during city production.
- Withdrawn quantities now produce an explicit economic-recheck fulfillment
  log.
- Queue insertion remains checked and each successful commitment still moves
  one quantity from pending to training.

The tender repair passes Debug and Release DLL builds and its Debug DLL is
deployed. Codex did not launch or advance the game.

- Tender repair commit: `fb79701d3`
- Latest Debug DLL SHA-256:
  `062F5983DF1D7D939A404DC2C83C7F1587D0DB4D715AC681D4AEFEC02A92BFE5`
- Latest Release DLL SHA-256:
  `4746A2BB459DDF644D30F8A3B6EF5671C3DACD994E5EC0DDBCED666B96551911`

## Superseded runtime gate

The user explicitly authorized Wave 2 implementation without another isolated
Wave 1 run. The tender repair therefore moved into the combined Wave 2
fresh-game test instead of being treated as a prerequisite. Its focused
confirmation criteria remain active in the Wave 2 verification checklist.
