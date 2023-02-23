# Operating Systems Assignment #1: <br>Shell - Command Execution and Composition
### Andrei Girjoaba -s4716825 <br> Oscar de Francesca -s4773012

<br>

---

## Bonuses
For this assignment we implemented numerous bonuses. We included 2 additional files *bonus.h* and *bonus.c*. <br>
The bonuses can be toggled on/off from defined value BONUS variable inside *bonus.h*.

~~~ C
#define BONUS 1
~~~

The code related to the bonus is wrapped in:

~~~ C
#if BONUS
    // some code
#endif
~~~

### Current Directory Path
We display in our shell the current directory path at all times. This gives our shell a more natural interface with which most people are used. <br> 
It also helps when navigating using the change directory command.

### Change Directory Command: *cd*
We fully implemented the *cd* command as a built-in. We display specific errors for *too many arguments*, *missing argument* or *directory not found*.

### Colors
To give a more unique feel to our shell we also implemented colors. We implemented unique colors for the current directory path and for the errors.