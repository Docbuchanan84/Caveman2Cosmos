# Domain map

Use this as a starting map, then trace the specific type/key/call before editing.

| Domain | Primary paths | Required focus |
|---|---|---|
| XML types | `Assets/XML`, `Assets/Modules`, `Sources/Cv*Info*`, `Sources/CvXMLLoadUtility*` | Schema validity, unique `Type`, references, load order |
| Localization | `Assets/XML/GameText` | Text key existence, language fallback, affected UI |
| Python | `Assets/Python/EntryPoints`, `CvEventManager.py`, `BUG/BugEventManager.py` | Civ IV Python 2 syntax, event registration, Python logs |
| UI | `Assets/Python/Screens`, `EntryPoints/CvScreensInterface.py`, `Assets/Config` | Screen lifecycle, resolution/input paths, option persistence |
| DLL rules | `Sources/CvGame*`, `CvPlayer*`, `CvTeam*`, `CvCity*`, `CvUnit*` | Release/Assert builds, Python exposure, save fields |
| AI | `Sources/*AI*`, `CvContractBroker*` | Scoring units, cache invalidation, RNG/determinism, turn cost |
| Options | `Assets/Config`, `Assets/Python/BUG`, `Sources/CvBugOptions*` | Default vs local value, synchronized vs UI-only behavior |
| Modular loading | `Assets/Modules`, `Sources/CvXMLLoadUtilitySet.cpp` | MLF/load ordering, duplicate types, modular art |
| Art | `UnpackedArt/art`, XML art definitions, `Tools/FPKLive.exe` | Source file plus path reference; generated FPK exclusion |
| Audio | `Assets/XML/Audio`, `Assets/Sounds` | Script/tag references, runtime audition, volume/category |

The table's path mapping is **[CODE]**. The validation focus is a **[LOCAL]**
development policy derived from those paths and the official developer workflow.

## Typical trace patterns

- XML key to rule: search the exact `<Type>`, find its info reader/accessor, then find
  consumers of the corresponding enum or getter. **[CODE]**
- Python callback: start at an engine entry point, follow
  `CvEventInterface`/`BugEventManager`, then inspect all registered handlers. **[CODE]**
- Screen behavior: start in `CvScreensInterface.py`, locate the screen instance and
  `interfaceScreen`/input/update methods, then trace `Cy*` calls into bindings.
  **[CODE]**
- C++ field: find construction/reset, mutation, checksum/RNG use, Python exposure,
  and every save read/write site before changing it. **[LOCAL]**

Do not directly edit generated DLL/PDB/FPK output to implement a source change.
**[CODE]**
