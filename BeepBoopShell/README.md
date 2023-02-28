# Operating Systems Assignment #1: <br>Shell - Command Execution and Composition
### Andrei Girjoaba -s4716825 <br> Oscar de Francesca -s4773012

<br>

---

## Bonuses
For this assignment we implemented numerous bonuses. We included 24 additional files *bonus.h*, *history.c*, *history.h* and *bonus.c*. <br>
The bonuses can be toggled on/off by compiling with the flag -DEXT_PROMPT. In our *Makefile*, we have the *bonus* command. Run:

~~~ 
make bonus
~~~

for the bonus.

The code related to the bonus is wrapped in:

~~~ C
#if EXT_PROMPT
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

### History
We implemented the built-in *hist*.  By typing:
~~~
hist
~~~
you display the history list of all previously executed comamnds.

 By typing:
~~~ 
hist n
~~~
where "n" is the index of the command in the history, you can execute a previously executed command.