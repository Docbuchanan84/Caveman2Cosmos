# AI Unit-Demand Wave 1 Runtime Test

Codex does not launch the game or run autoplay for this change. The user performs
the runtime steps; Codex can inspect the resulting local logs afterward.

## Build under test

- Branch: `codex/ai-unit-demand-accounting`
- Debug DLL: `Build/Debug/CvGameCoreDLL.dll`
- Release DLL: `Build/Release/CvGameCoreDLL.dll`
- Demand guard: `USE_AI_UNIT_DEMAND_ACCOUNTING` in
  `Sources/CvAIUnitDemand.h`

Deploy the Debug DLL through the normal local C2C workflow. Preserve the DLL
being replaced so the test can be rolled back immediately.

In BUG Options, open the Logging tab and set:

- Player BBAI log level: at least 1.
- City BBAI log level: at least 2.

This writes demand admission and fulfillment records to `ContractBroker.log`.

## Required Wave 1 gate

Use a representative save with several AI cities, preferably including at least
one coastal AI and more than one land area if available.

1. Load the save with the Debug DLL and advance five full turns.
2. Save under a new test filename, exit to desktop, reload that new save, and
   advance another five full turns.
3. Continue or autoplay for ten additional full turns.
4. If practical in the test position, include one city capture or raze and then
   advance at least two more full turns.
5. Stop immediately on an assertion dialog, repeatable crash, stuck turn, or
   obviously empty AI production.

Report:

- Original save name and starting turn.
- Turn reached before and after reload.
- Whether any assertion, crash, stuck turn, or production anomaly occurred.
- Which AI lost or captured a city, if that transition was exercised.

Leave the logs in place. Codex will inspect `ContractBroker.log` and the normal
debug logs for reconciliation failures and verify:

- Pending quantity never exceeds the target deficit.
- Repeated cities do not append duplicate player/area reservations.
- Successful fulfillment changes pending to queued training.
- Failed fulfillment remains uncommitted and disappears next cycle.
- Worker, sea-worker, hunter, settler, explorer, spy, and infiltrator demand
  stops when effective supply reaches its target.
- Save/reload does not cause a request or queue burst.
- Player, area, queue, broker-index, and water-cache assertions remain clean.

## Log code reference

Policy:

| Value | Policy |
|---:|---|
| 0 | Settler |
| 1 | Worker |
| 2 | Sea worker |
| 3 | Hunter |
| 4 | Land explorer |
| 5 | Sea explorer |
| 6 | Spy |
| 7 | Infiltrator |

Economy:

| Value | State |
|---:|---|
| 0 | Normal |
| 1 | Unit saturated |
| 2 | Financial trouble |
| 3 | Critical gold |
| 4 | Strike |

Admission:

| Value | Result |
|---:|---|
| 0 | Accepted |
| 1 | Refreshed |
| 2 | No deficit |
| 3 | Partially gated |
| 4 | Economically rejected |
| 5-8 | Invalid request or conflicting policy |

Fulfillment:

| Value | Result |
|---:|---|
| 0 | Committed |
| 3 | No eligible tender |
| 4 | No buildable unit |
| 6 | Economic recheck failure |
| 7 | Queue rejected |

## Wave 2 hold

Do not enable Wave 2 migrations until this Debug run, log review, and a Release
smoke run are accepted. After Debug acceptance, repeat the same save for five
full turns with the Release DLL and report any visible regression.
