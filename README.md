# blackjack

***Compile***: gcc blackjack.c -o blackjack

***Run***: ./blackjack

***Rules***

* Single deck
* Deck shuffled every 6 games
* Dealer hits soft 17

***Tests***
* Uncomment last lines in `main()` method to execute tests at the end of the game. Tests verify the behaviour of 3 important
helper functions `decideWinner`, `printTotal` and `isBusted`

***Winning rules***
https://en.wikipedia.org/wiki/Blackjack
* Players win by not busting and having a total higher than the dealer,
* or Player not busting and having the dealer bust,
* or Player getting a blackjack without the dealer getting a blackjack.
