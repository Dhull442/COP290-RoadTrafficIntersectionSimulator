# COP290-RoadTrafficIntersectionSimulator
SIM CITY

#### NOTE
- the co-ordinates are (0,0) at bottom left
- we will have to implement vehicular motion such that vehicles in front move first, so we know the space for next ones. (I have done basic unrestricted position to check new co-ordinates to update prev ones)
- Added speed limit ( right now same for all vehicles )
- can do toLowerCase in wehicle type to avoid any faltu errors
### Issues
- The defaults of vehicles must come from road;


### Enhancements
- Make multiple incomes in one second on road;
- can implement driving level as well: 0 - new, 1 - intermediate, 2 - expert;

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
even if the middle speed is more
