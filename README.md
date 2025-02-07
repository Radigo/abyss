"# abyss" 

Self-playing TGM bot

1 - A game: Should be playable as a TGM/TAP clone with human inputs.
2 - A screensaver: Should be able to play by itself with own bot.
3 - A demo: Bot should be able to play on given tempo.
4 - A lab: Should be able to plug external bot input for AI training?

1 - A game
We can choose between TGM and TAP ruleset to play a clone.
Inputs are keyboard only.

2 - A screensaver
Bot can enter inputs each frame to move pieces to target positions.

Issues:
Trouble moving pieces in target places, seems it can't place them in rightmost column.
Trouble finding the optimal position, lowest horizon seems badly evaluated.

Nice to have:
Step by step progress with a key (spacebar) to debug bot decisions.
Side window with debug info.

3 - A demo

4 - A lab

Misc
Make log to avoid manually including SDL everywhere
Performance issues on PC, need profiling.