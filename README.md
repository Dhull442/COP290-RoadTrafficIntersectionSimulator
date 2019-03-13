# COP290-RoadTrafficIntersectionSimulator
SIM CITY

#### NOTE
- the co-ordinates are (0,0) at bottom left
- we will have to implement vehicular motion such that vehicles in front move first, so we know the space for next ones. (I have done basic unrestricted position to check new co-ordinates to update prev ones)
- Added speed limit ( right now same for all vehicles )
- can do toLowerCase in wehicle type to avoid any faltu errors
- set default values to real life related
- have added a set of basic safety rules above to specify global defaults of speeds and acceleration
- -999 would be considered as not specified everywhere
### Issues

### Enhancements
- Make multiple incomes in one second on road;
- can implement driving level as well: 0 - new, 1 - intermediate, 2 - expert;
- can add deceleration as well.
- [ DONE ]
```
================
    |
    |
   |  
   |  |
      |
================
```
this shouldn't go to
```
================
        |
        |
          |
         ||
         |
================
```
even if the middle speed is more.
