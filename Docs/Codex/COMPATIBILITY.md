# Compatibility and safety

## Saves

Save compatibility is behavioral, not guaranteed merely because a build succeeds.
`CvTaggedSaveFormatWrapper` and the many object-level `read`/`write` implementations
make field names, defaults, remapped class types, and read/write symmetry material.
Test only copies of old saves and write new test filenames. **[CODE]**

For save-sensitive changes, record source SHA, build configuration, DLL SHA-256, save
origin/version, load result, turns completed, new-save result, and reload result.
**[LOCAL]**

## Multiplayer determinism

Use identical commits, XML/assets, options that affect rules, and DLL hashes on every
client. Route synchronized randomness through the established game RNG; do not let
local UI state, wall-clock time, filesystem order, or unordered iteration decide
game state. Compare OOS logs and checksums from the same turn when diagnosing.
`CvEventManager.py` and `OOSLogger.py` expose the current OOS logging path. **[CODE]**

## Caches and options

When changing a cached computation, identify every invalidation trigger and test both
a fresh calculation and an already-populated cache. Treat BUG/UserSettings values as
local until code proves they participate in synchronized state. Do not commit a local
`UserSettings` directory. **[LOCAL]**

## Performance

`Sources/fbuild.bff` supplies Profile and ProfileExtra configurations; native sources
contain `PROFILE_*` instrumentation, and `DevExtras/PyScripts/Profiling` contains
analysis helpers. Compare the same save, turn, player, options, and action sequence,
with warm-up and repeated measurements. **[CODE]**

## Generated files

`.gitignore` excludes `Build`, DLL/PDB, FPK, logs, `UserSettings`, `CHANGELOG.md`,
`mods_directory.txt`, FPKLive tokens, and several IDE/profiler outputs. An ignored
file is still operationally important: verify it, but do not stage it. **[CODE]**

The Git workspace is a real OneDrive directory and the active mod uses junctions
outward to it. Keep the workspace pinned locally and resolve every reparse point
before maintenance. **[LOCAL]**
