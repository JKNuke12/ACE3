
class CfgVehicles {
    class Man;
    class CAManBase: Man {
        class ACE_Actions {
            class GVAR(ReloadLauncher) {
                displayName = "$STR_ACE_ReloadLaunchers_LoadLauncher";
                selection = "launcher";
                distance = 4;
                condition = "";
                insertChildren = QUOTE(_this call FUNC(addMissileReloadActions));
            };
        };
    };
};
