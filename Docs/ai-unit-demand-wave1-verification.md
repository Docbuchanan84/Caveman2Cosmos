# AI Unit-Demand Accounting Wave 1 Verification

## Status

Partial runtime pass. The first 65-turn playtest validated production-demand
admission, economic gating, checked queue commitment, and bounded legacy broker
growth. It also exposed an initialization defect in the unsaved water-area
supplemental cache. That defect has been repaired and rebuilt, but the corrected
DLL still requires a focused fresh-start and save/reload retest before Wave 2.

Wave 2 remains gated.

## Test identity

- Test save: `UNIT_SPAM_FIX_TEST.CivBeyondSwordSave`
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

## Defect found and repaired

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

## Remaining runtime gate

Before Wave 2:

1. Start a fresh game with the corrected Debug DLL and advance at least two full
   turns.
2. Load `UNIT_SPAM_FIX_TEST`, advance at least five full turns, save under a new
   name, exit, reload that new save, and advance at least five more turns.
3. Stop on any assertion, crash, stuck turn, or empty AI production.
4. Verify the new logs contain zero `AI_UNIT_RECONCILE` records and no
   water-cache assertion.

After this focused retest passes, Wave 1 may clear its initialization and
save/reload gate. A later mature multi-city test is still required before final
MVP acceptance and the performance threshold remains unmeasured.
