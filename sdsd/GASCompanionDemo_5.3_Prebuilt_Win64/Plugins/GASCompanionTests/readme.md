Holds unit / functional tests for GAS Companion.

## Resources/AutomationTests

This folder holds assets for automation tests that are used part of the CI workflow.

Includes config files and a game feature with functional tests in it.

While working on it, simply copy over `Resources/AutomationTests/GameFeatures` to project host `Plugins/GameFeatures` and rename `GameFeature_GSC_Test.uplugin.json` to `GameFeature_GSC_Test.uplugin`

```bash
# From project host root
cp -r Plugins/GASCompanion/GASCompanionTests/Resources/AutomationTests/GameFeatures Plugins/GameFeatures
mv Plugins/GameFeatures/GameFeature_GSC_Test/GameFeature_GSC_Test.uplugin.json Plugins/GameFeatures/GameFeature_GSC_Test/GameFeature_GSC_Test.uplugin

# include config in host project
cat Plugins/GASCompanion/GASCompanionTests/Resources/AutomationTests/Config/DefaultGame.ini > Config/DefaultGame.ini
cat Plugins/GASCompanion/GASCompanionTests/Resources/AutomationTests/Config/DefaultInput.ini > Config/DefaultInput.ini
```

The Game Feature .uplugin file has `EnabledByDefault` set to true, so host project doesn't have to update its .uproject to enable it.

When you're done working with updating the tests, copy over `Plugins/GameFeatures` from host project over to `Resources/AutomationTests/GameFeatures` and rename the .uplugin file to `.uplugin.json`

### `Resources/AutomationTests/GameFeatures`

Include game feature plugins used as part of the functional tests. The host project running the tests have to copy this folder over from `Resources/AutomationTests/GameFeatures` to project's `Plugins/GameFeatures`

Has to happen before launching the editor to run the tests.

.uplugin file is named `.uplugin.json` to make sure engine don't see it when recursively searching for plugins (This gets rid of a warning when compiling if the Test Game Features has been copied to project's `Plugins/GameFeatures`). It has to be renamed once to `.uplugin` after getting copied to project `Plugins/GameFeatures` destination.

### `Resources/AutomationTests/Config`

#### `Resources/AutomationTests/Config/DefaultGame.ini`

Includes a `DefaultGame.ini` file with the minimal amount of config to run the tests. Includes the `Engine.AssetManagerSettings` explained below.

This file can be used as is in a HostProject used to the tests run within (CI workflow uses TP_BlankBP template from engine)

#### `Resources/AutomationTests/Config/DefaultInput.ini`

Includes a `DefaultInput.ini` file with the minimal amount of config to run the tests. Includes the `Engine.InputSettings` explained below, with configuration for Enhanced Input (with engine 5.1, this will be not needed anymore as EnhancedInput becomes the default)

This file can be used as is in a HostProject used to the tests run within (CI workflow uses TP_BlankBP template from engine)

---

`Config/DefaultGame.ini` needs to include the following config:

```ini
[/Script/Engine.AssetManagerSettings]
-PrimaryAssetTypesToScan=(PrimaryAssetType="Map",AssetBaseClass=/Script/Engine.World,bHasBlueprintClasses=False,bIsEditorOnly=True,Directories=((Path="/Game/Maps")),SpecificAssets=,Rules=(Priority=-2,ChunkId=-1,bApplyRecursively=True,CookRule=Unknown))
-PrimaryAssetTypesToScan=(PrimaryAssetType="PrimaryAssetLabel",AssetBaseClass=/Script/Engine.PrimaryAssetLabel,bHasBlueprintClasses=False,bIsEditorOnly=True,Directories=((Path="/Game")),SpecificAssets=,Rules=(Priority=-2,ChunkId=-1,bApplyRecursively=True,CookRule=Unknown))
+PrimaryAssetTypesToScan=(PrimaryAssetType="Map",AssetBaseClass="/Script/Engine.World",bHasBlueprintClasses=False,bIsEditorOnly=True,Directories=((Path="/Game/Maps")),SpecificAssets=,Rules=(Priority=-2,ChunkId=-1,bApplyRecursively=True,CookRule=Unknown))
+PrimaryAssetTypesToScan=(PrimaryAssetType="PrimaryAssetLabel",AssetBaseClass="/Script/Engine.PrimaryAssetLabel",bHasBlueprintClasses=False,bIsEditorOnly=True,Directories=((Path="/Game")),SpecificAssets=,Rules=(Priority=-2,ChunkId=-1,bApplyRecursively=True,CookRule=Unknown))
+PrimaryAssetTypesToScan=(PrimaryAssetType="GameFeatureData",AssetBaseClass="/Script/GameFeatures.GameFeatureData",bHasBlueprintClasses=False,bIsEditorOnly=False,Directories=((Path="/Game/Unused")),SpecificAssets=,Rules=(Priority=-2,ChunkId=-1,bApplyRecursively=True,CookRule=AlwaysCook))
bOnlyCookProductionAssets=False
bShouldManagerDetermineTypeAndName=False
bShouldGuessTypeAndNameInEditor=True
bShouldAcquireMissingChunksOnLoad=False
bShouldWarnAboutInvalidAssets=True
MetaDataTagsForAssetRegistry=()
```

`Config/DefaultInput.ini` needs to include the following config:

```ini
[/Script/Engine.InputSettings]
DefaultPlayerInputClass=/Script/EnhancedInput.EnhancedPlayerInput
DefaultInputComponentClass=/Script/EnhancedInput.EnhancedInputComponent
```

## Notes

Test: ✔️

Ability Sets

- Game Feature
  - PlayerState (Active, Activated during play, Deactivate during play)
    - ✔️ Standalone
    - ✔️ Listen Server
    - ✔️ Client
  - Pawn (Active, Activated during play, Deactivate during play)
    - ✔️ Standalone
    - ✔️ Listen Server
    - ✔️ Client
- ASC Class Defaults (Init time)
  - PlayerState 
    - ✔️ Standalone
    - ✔️ Listen Server
    - ✔️ Client
  - Pawn
    - ✔️ Standalone
    - ✔️ Listen Server
    - ✔️ Client
- During runtime (not on spawn, on input for instance)
  - PlayerState
    - Standalone
    - Listen Server
    - Client
  - Pawn
    - Standalone
    - Listen Server
    - Client

Grant Ability With Input

- Runtime (not on spawn, on input for instance)
  - PlayerState
    - Standalone
    - Listen Server
    - Client
  - Pawn
    - Standalone
    - Listen Server
    - Client
- Init Time
  - PlayerState
    - Standalone
    - Listen Server
    - Client
  - Pawn
    - Standalone
    - Listen Server
    - Client