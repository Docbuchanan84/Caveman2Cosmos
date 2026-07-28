# Architecture

Baseline: `4f6bf1b0a132e5aea6028f2a78ebd2a2bd1708e5`.

## Runtime layers

1. **XML content.** `Assets/XML` is divided into art, audio, basic info, buildings,
   civilizations, events, game info, text, interface, technologies, terrain, units,
   and schemas. The DLL loads and maps info types through the `CvXMLLoadUtility*`
   family; modular controls are implemented in
   `Sources/CvXMLLoadUtilitySet.cpp`. **[CODE]**
2. **Python behavior and UI.** Engine entry points live in
   `Assets/Python/EntryPoints`. `CvAppInterface.py` crosses the application boundary,
   `CvEventInterface.py` exposes the event manager, `BUG/BugEventManager.py` extends
   `CvEventManager.py`, and `EntryPoints/CvScreensInterface.py` dispatches screen
   calls into `Assets/Python/Screens`. **[CODE]**
3. **Native game core.** `Sources` builds `CvGameCoreDLL.dll`. Game objects and rules
   are concentrated in `CvGame`, `CvPlayer`, `CvTeam`, `CvCity`, `CvUnit`, their AI
   counterparts, info classes, and Python-facing `Cy*` wrappers. Bindings are
   registered from `Sources/CvDLLPython.cpp`. **[CODE]**
4. **Beyond the Sword engine.** The closed engine loads XML, Python, and the mod DLL,
   drives rendering/networking, and calls the exported interfaces. Headers such as
   `CvDLLUtilityIFaceBase.h` describe the DLL-side contract but not the engine
   implementation. **[CODE]**
5. **Assets.** Source art belongs in `UnpackedArt/art`. In a development install,
   `Sources/CvGameCoreDLL.cpp` detects `git_directory.txt`, runs
   `Tools/FPKLive.exe`, and then checks the DLL before normal startup. Generated
   `Assets/*.FPK` files are ignored. **[CODE]**

## Data flow

XML definitions and module files are parsed into C++ info tables. Game-state C++
objects consume those tables and expose selected operations through `Cy*` bindings.
Python entry points attach events, options, and screens to those bindings. The engine
loads runtime art/audio paths referenced by XML and drives the final interface.
**[CODE]**

BUG configuration definitions live in `Assets/Config`; the Python implementation is
under `Assets/Python/BUG`. Runtime option values are local state and are not content
definitions. **[CODE]**

## Persistence and synchronization

Major game objects implement `read(FDataStreamBase*)` and
`write(FDataStreamBase*)`. Many use `CvTaggedSaveFormatWrapper` and `WRAPPER_*`
macros, for example `CvArea.cpp`, `CvCity.cpp`, and `CvCityAI.cpp`. A field change is
therefore a save-compatibility task, not just a class-layout edit. **[CODE]**

Game-affecting randomness commonly flows through `CvGame::getSorenRandNum`; OOS
diagnostics are exposed to Python through `Assets/Python/OOSLogger.py` and invoked
from `CvEventManager.py`. Avoid client-local or iteration-order-dependent decisions
in synchronized logic. **[CODE]**

## Generated boundaries

The build pipeline writes `Build/<Config>`, then deploys DLL/PDB files into `Assets`.
Visual Studio user settings, FPK packs, logs, local settings, and release metadata are
generated/local-only according to `.gitignore` and the setup scripts. **[CODE]**
