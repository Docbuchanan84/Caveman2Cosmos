# AI Unit-Demand Wave 2 Verification

## Scope

Wave 2 extends authoritative production demand to exact spread units and
explicit military, naval, air, and nuclear targets. It adds formation-aware
Size Matters accounting and capacity/volume accounting for transports and
carrier payloads.

The implementation deliberately leaves exact-city defenders, property
responders, healer support, tactical strength/odds, rebel production, and
NPC/barbarian immediate production on their existing paths.

## Static and build verification

- The implementation is behind `USE_AI_UNIT_DEMAND_WAVE2`.
- Production reservations and measured caches remain unsaved.
- `AI_totalUnitAIs()` and existing area-total functions are unchanged.
- Exact UnitType pending reservations have a separate deterministic index.
- Tender revalidation can shrink but cannot expand admitted demand.
- A successful measured commitment removes no more than the selected concrete
  unit's contribution and queue insertion updates measured training supply.
- Queue push, pop, and upgrade paths update measured training supply.
- Unit movement, completion/creation, role changes, death, merge/split group
  changes, and Size Matters cargo changes update measured live supply.
- Debug and Release FASTBuild configurations complete with `FBuild: OK`.
- The pre-existing `CvUnitAI.cpp` boolean-truncation warning remains.
- Release retains the pre-existing `VERSION.dll` `/OPT:REF` warning.
- FASTBuild may report the known OneDrive `.tmp` database rename message after
  a successful build.

Codex did not launch or advance the game. Runtime acceptance is assigned to the
user playtest.

## Required combined playtest

Use a fresh game with Size Matters enabled and logging enabled.

1. Play at least 100 turns, saving a checkpoint before exiting.
2. If possible, merge three same-role companies into one battalion. The merge
   must not trigger two replacement orders for that role.
3. Exit, reload the checkpoint, and play at least 20 more turns.
4. Stop immediately on an assertion, crash, stuck turn, or cities unable to
   choose production.
5. Report the save name, approximate turn, and any conspicuous repeated unit
   streak.

For log review, verify:

- zero `AI_UNIT_RECONCILE` records and zero asserts;
- each request commits no more than its admitted measured quantity except one
  concrete-unit granularity overshoot;
- successful commitment converts pending supply into training supply;
- exact spread demand admits no more than one new unit per exact UnitType per
  production cycle;
- formation supply remains stable across merge/split;
- strategic/optional military production stops at its target or economic gate;
- transient no-producer/queue-rejected demand does not survive the next cycle;
- broker growth remains proportional to active needs.

## Still outstanding after the first playtest

- A mature naval/air/nuclear save is desirable for late-game coverage.
- A Size Matters-disabled comparison run is desirable.
- Identical-save checksum comparison and the five-percent median turn-time gate
  remain final acceptance work.
