/*
  Name: FUNC(isEOD)

  Author: Garth de Wet (LH)

  Description:
    Checks whether the passed unit is an explosive specialist.
    Either through config entry: "canDeactivateMines"
    or
    unit setVariable ["ACE_isEOD", true]

  Parameters:
    0: OBJECT - Unit to check if is a specialist

  Returns:
    BOOLEAN

  Example:
    _isSpecialist = [player] call FUNC(isEOD);
*/
#include "script_component.hpp"

private "_unit";

_unit = _this select 0;

_unit getVariable ["ACE_isEOD", getNumber (configFile >> "CfgVehicles" >> typeOf _unit >> "canDeactivateMines") == 1]
