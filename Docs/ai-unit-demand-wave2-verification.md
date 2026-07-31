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

Final pre-deployment build evidence:

- Accounting foundation: `ab52ba23f6ad5b7e18b38c8dd079657d84b8a6d6`
- Wave 2 migrations: `3676dc96b11ba27ec61cb09f512b6b4106a7b622`
- Formation-heuristic normalization:
  `938b7a993186d10770b17a95dc5add54a380e2a9`
- Debug DLL SHA-256:
  `1D9AA34FC7551F57035ED6327E84062028F56A11C7B64E4F2524A32DDF44A71A`
- Release DLL SHA-256:
  `58F256621DD84EF15C29236A0BDE3A49C55514C9D82EF238B6C03C65BDC7F799`
- The Debug DLL was deployed through the workspace `Assets` path and verified
  byte-identical through the active GOG mod junction.
- The previous Wave 1 DLL and both Wave 2 build configurations were preserved
  under `D:\C2C-Backups\ai-unit-demand-wave2-20260730-193555`.

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

## Turn-7 Debug incident and corrective build

The first fresh Wave 2 autoplay stopped on turn 7 at
`CvPathGenerator.cpp:1505` while a barbarian attack unit compared paths to
adjacent plots around its target city. The assertion predates this branch and
is unchanged at the accepted predator baseline. The Release path already
relinks and repairs the cheaper cached route. The Debug assertion now permits
that valid cheaper-route case while retaining the non-increasing-cost
invariant.

The same run also exposed a Wave 2 accounting defect before the visible
assertion: new-unit initialization and Size Matters recalculation could
subtract a live measured contribution that had not yet been registered. Live
measured accounting now stores an unsaved snapshot per unit. A mutation removes
that exact prior snapshot and then registers the unit's current role, area,
formation volume, cargo capacity, and cargo volume. Zero-valued cache entries
are removed canonically, and cargo load/unload changes update the transport's
snapshot.

Captured logs are preserved locally under
`D:\C2C-Backups\unit-demand-assert-20260730-222142`.

Corrective build evidence:

- Clean Debug rebuild: `FBuild: OK`.
- Release build: `FBuild: OK`.
- Corrective Debug DLL SHA-256:
  `9C2FF0F4B70B223297653EF8B97A69A2832C8D29EF244B0500D6762AD8ED2FE4`.
- The corrective Debug DLL is byte-identical through the active GOG mod
  junction.
- Codex did not launch or advance the game.

Corrective retest:

1. Start a fresh Size Matters game named
   `UNIT_SPAM_FIX_WAVE2_ASSERT_RETEST`.
2. Automate toward 100 turns.
3. Stop immediately on any assertion, crash, or stuck turn.
4. If turn 100 is reached, create a named save and exit normally.
5. Report the reached turn and any conspicuous repeated-unit production.

Acceptance requires no `AI_UNIT_RECONCILE` records and no repeat of the
known-route path assertion.
