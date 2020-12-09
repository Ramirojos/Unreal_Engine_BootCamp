# Unreal_Engine_BootCamp
Contents of a game made with UE4 for a Globant BootCamp. Mainly made in C++, some things are made with the UE Blueprints system.

The objective os the game is to collect enough treasure (default:2500) while keeping the player's healt above 0. Once you collect the specified ammount, the Win condition will be met and the "Win Screen" will appear. If the Player's health drops to 0, then the "Lose Condition" triggers and the "Lose Screen" will appear.

Both those screens, and the "Main Menu" have functioning Buttons.

All of the Key bindings are configurable on the project Settings.

##Content made in C++:
  * Player Character (movement, health, pickup collection system).
  * Player Cameras ( First and Third person, you can toogle between them).
  * Pickups.
  * Vertical and Horizontal Floating Platforms.
  * Gamemode.

##Content made in Blueprints:
  * Enemy AI ( patrol, player detection, damage dealing).
  * Player & Game UI (Player HUD, Main Menu, win & lose screens).
  * Poisonous gas area ( on overlap damage player, particle system).


##Epic gamestore Assets used in this project (all free):
  * Infinity Blade Adversaries.(https://www.unrealengine.com/marketplace/en-US/product/infinity-blade-enemies)
  * Paragon: The Fey.(https://www.unrealengine.com/marketplace/en-US/product/paragon-the-fey)
  * Ancient Treasures.(https://www.unrealengine.com/marketplace/en-US/product/9efde82ef29746fcbb2cb0e45e714f43)
  * Medieval Dungeons. (https://www.unrealengine.com/marketplace/en-US/product/a5b6a73fea5340bda9b8ac33d877c9e2)
  * Infinity Blade: Effects (https://www.unrealengine.com/marketplace/en-US/product/infinity-blade-effects)
  * Open World Demo Collection (https://www.unrealengine.com/marketplace/en-US/product/open-world-demo-collection-01)
