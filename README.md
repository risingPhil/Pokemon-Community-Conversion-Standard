<img width="2209" height="1168" alt="pccs" src="https://github.com/user-attachments/assets/271c46a4-d2b0-49df-a54c-d70d69dd5af4" />
The Pokemon Community Conversion Standard is a C++ library designed to have a small handful of standard ways to convert a Pokemon from Generation 1 or Generation 2 to Generation 3 that are agreed upon by the community. It is by no means official, but is meant to become a standard that the community can agree on. Not every method is perfect for every situation, so there are a few options. This is meant to be a _community_ standard, so please feel free to suggest changes and modifiations! Here is the current list of conversion methods:

## FAITHFUL:
This method is designed to keep as much original information about your Pokemon as possible, even if it might cause a few issues down the road. For instance, a Generation 2 Pokemon transferred with this method will have a Game of Origin of HeartGold. While this Pokemon will be legal if transferred immediately to Generation 4, it would become illegal if it recieved the Champion Ribbon beforehand. Mythical Pokemon are treated the same as all other Pokemon and not modified to match their events.

## LEGAL:
This method is designed to make the transferred Pokemon indistiguishable from a completely normal Generation 3 Pokemon. This will sacrifice specific things (such as Game of Origin) for the peace of mind knowing that you will have no issues bringing your Pokemon to the most recent generation. Mythical Pokemon are converted into their event forms in order to maintain transferrability.

## VIRTUAL:
This method replicates the conversion method used by Poke Transporter when bringing Pokemon from the Virtual Console releases of Red, Blue, Yellow, Gold, Silver, and Crystal into Pokemon Bank. Some aspects such as the GameBoy Origin Mark and hidden abilities are not carried over, due to not existing in Generation 3. Mythical Pokemon will be treated as any other Pokemon, outside of getting 5 max IVs instead of 3.

## ORIGINAL:
This method was created for the original release of Poke Transporter GB and is kept as a legacy conversion method. This mostly follows the Virtual Console method, but will sanitize mythical Pokemon into their event forms.

## Full Conversion Method Charts:

| |Faithful|Legal|Virtual|Original|
|---|:---:|:---:|:---:|:---:|
|Language||||Language is set to match the source game. Korean Pokemon sent from Gold or Silver are set to Japanese and given their default Japanese names|
|Friendship||||Friendship is set to 70|
|Pokerus||||Pokerus strain and days remaining is maintained|
|Size||||Size is set randomly|
|Item||||Items are removed from the Pokemon|
|Ability||||Ability is set randomly|
|Nature||||Assigns based on the modulo of the total EXP, before being truncated|
|EXP||||Truncates the EXP down to the current level|
|Personality Value||||Creates a valid personality value based on the given gender, nature, and Unown form (if relevant)|
|Trainer Info||||Copies the Original Trainer and Trainer ID directly. Mythical Pokemon are given their event OTs and IDs|
|Pokeball||||All Pokemon will be in a Pokeball|
|Game of Origin||||Pokemon transferred from Red, Blue, or Yellow will have FireRed as their Game of Origin and Pokemon transferred from Gold, Silver, or Crystal will have HeartGold as their Game of Origin. Mythicals will match their event forms' Game of Origin|
|Met Location||||Pokemon will be labeled as met in a fateful encounter|
|Nickname||||Nicknames are maintained. Mythical Pokemon nicknames are not overwritten (since it is techincally possible to RNG the same OT and TID)|
|Shininess||||Shininess is maintained. Mythical Pokemon are made no longer shiny|
|Moves||||Moves that are not learnable by the Pokemon in Generation 3 are removed. A Pokemon with no moves is given the first move in their level up moveset|
|Ribbons and Fateful Encounter||||No ribbons are given. Mythical Pokemon have their Fateful Encounter flag set|
|EVs||||EVs are wiped|
|IVs||||IVs are set randomly, but will match the PID RNG|
|PP UPs||||PP Ups are maintained|
