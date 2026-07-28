# Glossary

- **Assert build** — FASTBuild configuration with runtime assertions enabled; defined
  in `Sources/fbuild.bff`. **[CODE]**
- **BUG** — the Python/XML configuration and UI framework under `Assets/Python/BUG`
  and `Assets/Config`; not a defect label in this context. **[CODE]**
- **BtS** — Civilization IV: Beyond the Sword, the host executable/engine.
- **C2C** — Caveman2Cosmos.
- **Cy\*** — Boost.Python-facing wrapper types exposing selected DLL functionality to
  Python; registered from files including `CvDLLPython.cpp`. **[CODE]**
- **DLL** — `CvGameCoreDLL.dll`, the native game-rule module built from `Sources`.
  **[CODE]**
- **FPK** — packaged Firaxis asset output. In the development install, FPKLive creates
  ignored runtime packs from source art. **[CODE]**
- **FPKLive** — tracked tool started by the development DLL when
  `git_directory.txt` is present. **[CODE]**
- **MLF** — modular loading control handled by `CvXMLLoadUtilitySet.cpp` and
  `MLF_CIV4ModularLoadingControls.xml` files. **[CODE]**
- **OOS** — out of sync: multiplayer clients disagree about synchronized game state;
  diagnostics are written by `Assets/Python/OOSLogger.py`. **[CODE]**
- **Soren RNG** — synchronized game random-number path exposed through
  `CvGame::getSorenRandNum`. **[CODE]**
- **Tagged save wrapper** — C2C serialization layer implemented by
  `CvTaggedSaveFormatWrapper` and used through `WRAPPER_*` macros. **[CODE]**
- **UnpackedArt** — tracked source-art root; generated FPK output does not belong
  there or in commits. **[CODE]**
- **UserSettings** — local BUG/mod option state, excluded from version control.
  **[CODE]**
