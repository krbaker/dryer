# WARNING *It is way safer to not use this code* 
(and possibly even to not use this product)

If you were to use this you could Plug in a DrySafer Dryer Alarm which might detect overheating and lint build up.
[Like This](https://www.walmart.com/ip/DrySafer-Dryer-Alarm-Detects-Overheating-and-Lint-Build-up-Prevents-Dryer-Fires/951064967)

I built this by reverse "engineering" the output cable for the drysafer elbow.  The drysafer basically outputs the audible buzzing pattern directly from the elbow electronics and I'm using a pin on the arduino to capture that.  This code uses the self test at startup to ensure it can 'hear' the buzzing from the elbow.  It listens for the other patterns (clog, high heat) and reports either one in theory.

*NOTE* This is not even *close* to a lifesaving device!?!?!?! I was surprised when I got one of these that it puts a flap directly in the exit path from the dryer so this is probably worse than nothing.  Use at own riks.  This code also has bugs.  Its also interesting to me that the clog alarm can be set off if you don't put the elbow at a perfect 90% which means it is going to behave differently with minor installation changes (oof).
