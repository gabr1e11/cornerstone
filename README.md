# cornerstone

Toy sandbox framework to try out game ideas. It is loosely inspired in the Unity API and it tries
to just be easy to use, not performant in particular.

The framework is composed by several systems working together:

## Ent, the entity-component system

Basic ECS inspired in the Unity API that brings basic features to help make games:
    * Entity based component system very similar to that in Unity
    * Basic components provided: SpriteRenderer, TextRenderer, MouseHandler, BounceUpDownAnimation, etc...
	* Entity life-cycle similar to that in Unity (init, start, update)
	* Entity lifetime managed through smart pointers
	* Iterable enumerations


## Prism, the rendering engine

Currently it provides a basic SDL rendering engine enough to make simple games and to be able to
test and debug the Ent system. In the future I will port my OpenGL renderer with programmable
pipeline support, procedural geometry and some other "niceties". At the moment it supports:
 	* Scaling and rotation of sprites
	* Opacity for sprites and text
	* Basic texture cache
	* Added possibility to tint sprites and text
	
## Future work

My idea is to continue improving the entity system to make it more complete, and then start porting my old
OpenGL engine as Prism. The main focus is on 2D for now, but I'd like to have a 3D renderer behind to toy
with 2D/3D integration while keeping the game 2D. I also want to invesigate pixel art techniques that simulate
3D effects in 2D
