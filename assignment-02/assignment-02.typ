#import "../template.typ": template
#show: template.with(
  title: "CS51012: GPU Programming and Real-Time Rendering",
  assignment: "Assignment Part 2",
  abstractTitle: "GPU Programming and Real-Time Rendering",
)

#let kwblue(body)   = text(fill: rgb("0064DC"), weight: "bold", body)
#let kwred(body)    = text(fill: rgb("C83232"), weight: "bold", body)
#let kwgreen(body)  = text(fill: rgb("008C46"), weight: "bold", body)
#let kwpurple(body) = text(fill: rgb("8C32B4"), weight: "bold", body)
#let kworange(body) = text(fill: rgb("DC7800"), weight: "bold", body)
#let kwteal(body)   = text(fill: rgb("008C8C"), weight: "bold", body)
#let kwmaroon(body) = text(fill: rgb("963C5A"), weight: "bold", body)
#let kwolive(body)  = text(fill: rgb("647800"), weight: "bold", body)

#image("img/screenshot.png", width: 100%)
#v(1em)

#columns(2, gutter: 16pt)[
= Introduction
== The Empty Cradle

#v(1em)

_Abandoned, but not yet forgotten #sym.dots _

The fantasy scene shows an empty cradle, abandoned in the corner of a room, but
not yet forgotten by the glowing orbs that float above and around it as they
drop magical dust where they wander. The cradle gently rocks, and the mobile
continues to rotate above. The ornaments naturally spin back and forth, and the
train's wheels turn.

The scene is intentionally dark, to show the lighting effect of the orbs.
However a stronger light can be toggled on and off using the *L* key, and moved
around the scene using the *keypad* (see @sec:instructions). A white sphere
represents its location.

An adventure torch is also available, and can be toggled on and off using the
*T* key. Coloured lens filters can be used by scrolling the mouse-wheel, and
it's beam width adjusted using the *[* and *\]* keys.

The cot mobile is made from geometric shapes. The remainder of the scene is
loaded from `.obj` files. The cradle and golf ball (next to the television) use
diffuse and normal textures from the `.obj` files, while the rest of the loaded
scene uses diffuse colours from the files.

The scene uses graph structures, which includes transformation, animation, and
particle emission functionality.

The main OpenGL techniques added to the second assignment include:
+ skybox
+ particle emission --- instanced, with randomised attributes which adjust
  according to each particle's time-to-live
+ model loading --- using ASSIMP
+ material loading --- using ASSIMP
+ normal mapping --- using tangents and bitangents
+ tone mapping --- using Reinhard, ACES @aces, or Uncharted2 @uncharted2
  algorithms
+ bloom effect --- using Optimised 3-, 5-, 7-, or 9-Step Kernel weights

A complete list of features is available in @sec:key-features, with new features
labelled appropriately.

== What More Would I Have Liked To Do

I tried to add the scene title to the wall as a texture, using Blender. However
my laptop is eleven years old, and Blender runs out of memory if I use it too
long or try to do too much. As I was unable to do this, I instead used a decal
mesh object that I created for the mobile star ornament, and transformed it in
the scene appropriately, showing only one of its sides.

The particles were emitted in clumps, due to the time delta between frames. I
managed to mostly prevent this by interpolating the orb's change in location and
breaking up the time delta. I also added some randomisation to each particle's
y-coordinate. Sometimes a clump is visible, though this may be a natural clump
formed by the orb's change of direction.

I would have liked to have done more, but did not have the time:
- made some improvements to the `glo` library that I created
- implement shadows
- use name tags for my scene graph to identify nodes, and automatically insert
  them from the `.obj` files, thus allowing more robust graph navigation

= Instructions <sec:instructions>
== Build & Run

The application is built using `cmake`. It should be run from the project's root
folder.

```bash
$ cmake -B build
$ make -C build
$ ./assignment-02
```

== Usage

*Movement:* #kwblue[WASD] keys for movement. #kwblue[R] and #kwblue[F] to move
up and down. Move the mouse to change camera rotation.

*Torch:* #kwred[T] to toggle the torch's power, #kwred[\[] and #kwred[\]] to
tighten and widen the beam. Use the mouse #kwred[scroll wheel] to rotate through
the different coloured lens filters.

*Main Light:* #kwgreen[L] switches the main light off and on. The
#kwgreen[keypad] numbers #kwgreen[8], #kwgreen[4], #kwgreen[6], and #kwgreen[2]
can be used to move the main light along the x and z axis. #kwgreen[9] and
#kwgreen[3] will move it up and down the y axis. #kwgreen[Keypad 5] will reset
the light to its initial location.

*Orbs:* #kwpurple[O] pauses the animation of the orbs. #kwpurple[Shift+O]
toggles their visibility (and their light) entirely.

*Mobile:* #kworange[M] pauses the rotation of the cot mobile. #kworange[P]
pauses the rotation of the mobile, the ornaments, and the cradle.

*Animation:* #kwteal[Keypad $+$] and #kwteal[$-$] adjusts all animation speeds.

*Particles:* #kwmaroon[E] pauses the particle emission.

*Phong Specular Method:* #kwolive[F5] switches between Blinn Phong Specular and
Phong Specular.

*Bloom:* #kwblue[F6] and #kwblue[Shift+F6] adjusts bloom level (number of
Gaussian blur taps).

*Tone Mapping:* #kwred[F7] and #kwred[Shift+F7] cycles between tone mapping
techniques.

*Normal Mapping:* #kwgreen[F8] toggles off and on normal mapping.

*Exit:* #kwpurple[Escape] exits the program.

== Key Features <sec:key-features>

+ The cot mobile rotates continuously at a constant speed in one direction on
  the y-axis.

+ Ornaments rotate "naturally" in alternating directions on the y-axis. Speed is
  slowed at the end of a rotation, and increased again when the direction has
  changed.

+ Train wheels rotate continuously at a constant speed in one direction on the
  z-axis.

+ Primary lighting provided by five floating orbs, which move above ornaments
  along randomised Bezier curves.

+ Additional light source (switched off by default) provides stronger light.

+ Weak light also provided by the "adventure" torch.

+ The torch's beam may be made wider or tighter.

+ Tighter torch beams have longer reach than wider beams.

+ The torch's colour filters may be changed using the mouse scroll wheel.

+ WASD and RF (up/down) movement, with viewing angle controlled by the mouse.

+ *NEW* Cot mobile rendered in a loaded model of a room. Each mesh in the room
  is coloured according to the `.obj` file diffuse colour.

+ *NEW* A cradle is located below the cot mobile. This is loaded from a `.obj`
  file with textures. An animator is used to gently rock the cradle.

+ *NEW* A skybox of the Milky Way is added.

+ *NEW* The orbs have a particle trail. The particles use an instancing method.
  Each particle has a randomised yellow-gold colour, and a limited life during
  which time they slowly fall, fade, and get smaller. The starting position is
  interpolated over the frame's time delta. These can be paused using E, causing
  existing particles to be frozen, and no new particles to be emitted. The
  length of the trail is long to allow the drop to be clearly visible.

+ *NEW* A Bloom effect is added to the orbs. The effect can be adjust up or down
  (circular) using F6 and Shift-F6 respectively.

+ *NEW* Tone mapping added, using Reinhard, ACES, and Uncharted2 techniques.
  These can be cycled through, or turned off, using F7 and Shift-F7.

+ *NEW* Normal mapping implemented for loaded meshes, using tangents and
  bitangents. This can be toggled off and on using F8. A golf ball was added to
  the scene, next to the television, as this shows the normal mapping more
  clearly --- especially by moving the main light to each side of it.

= Decision Rationale
== Scene Graphs

The `SceneNode` class can be used to create a graph of `Renderable` objects.
Each node may optionally have a `Renderable`, and zero or more child
`SceneNodes`. An empty node is valid, yet pointless. Using a graph provides easy
management of mesh construction, storage, and destruction. And provides
efficient transformation, animation, and particle emission.

Primarily, the graph allows a transformation in a parent node, to be reflected
in all descendant nodes, without setting each separately --- translating,
scaling, and/or rotating a parent, will provide a transformation matrix for each
descendant with the same transformations.  The transformations are one way, from
parent to child, allowing further transformations in the children without
affecting parents, cousins, etc.

Methods are provided to allow relative and absolute transformations (e.g.
`rotate` and `setRotation`), and access to each transformation (e.g.
`getRotation`).

Each `SceneNode` may have zero or more animations. These animations are updated
after every call to `animate`. An animation should update the `SceneNode`'s
transformation according to its purpose, taking advantage of the transformation
mechanics described above.

Each `SceneNode` may also have one or more particle emitters. These are updated
after every call to `emit`.

By using shared pointers, the `SceneNode` graph also allows an easy way to store
all scene objects. The client may choose to keep its own shared pointer of a
node for later access, or access it from the graph using the `getChildren`
accessor.

Rendering a `SceneNode` does _not_ render the children. Graph traversal should
be done manually, to allow shader uniform values to be updated appropriately. A
call to `updateTransformation` should be made once, to the top parent node,
before the rendering loop begins. This allows any nodes marked as dirty to
update their transformation matrix just once. Rendering a mesh is done by a call
to `SceneNode.render`. Rendering particles should be done after all `render`
calls are complete, using `SceneNode.renderParticles`.

== Triangle Arrays

My own research into OpenGL practices has led me to understand that modern GPU
architectures are optimised for triangle arrays. By taking advantage of this, we
no longer have to specify degenerate triangles, and only need to make one call
for the whole sphere to the GPU, instead of one call per ring. This all comes at
the cost of a higher number of vertices. However, in modern GPUs, space is no
longer the premium that it once was.

= Source Materials

In addition to attending lectures, I also used Learn OpenGL @guide and OpenGL
Programming Guide @kessenichOpenGLProgrammingGuide2017 to learn in my own time.

== `glo/camera.*`

I asked DeepSeek @DeepSeek to generate code for the `lookAt` method, which
changes the camera's yaw and pitch according to the direction of the target
location. Apart from modifying the code to fit into the Camera class, and adding
a clamp, no other modification was required.

== `glo/mesh/shapes/cylinder.*`

Although I have written this class myself, to aid in improving my usage of sin
and cos, I did at times refer to how `SphereMesh` and `StarMesh` work, which
themselves are modifications of DeepSeek generated code.

== `glo/mesh/shapes/sphere.*`

I asked DeepSeek to generate code for a sphere, using a triangle array. The code
was modified to cater for:

- efficiency --- the code was cleaned up, duplication removed, and value
  calculations minimised (for example, y only has to be calculated once per
  ring).
- types --- the data types were changed to use OpenGL types (for example, from
  `float` to `GLfloat`).
- winding --- winding had to be corrected.
- adaption --- the relevant code was extracted to place into the SphereMesh.
  Variable names were modified for consistency with the current codebase, and
  consts specified as appropriate.

== `glo/mesh/shapes/star.*`

I asked DeepSeek to generate code for several variations of cogs, as I was
considering project ideas and investigating the possibility of a bronze
clockwork mechanism. It had previously occurred to me that the geometric shapes,
with the test wraps I was using at the time, reminded me of a child's play room.
I realised that I could create a star mesh from similar code that DeekSeek had
generated for one of the cogs. And so though the generated code has been heavily
modified to suit the new shape, the original was generated by DeepSeek. The
method `generateEdge` remains mostly intact to what was generated, though
winding did need to be fixed. Overall, code was modified to cater for:

- efficiency --- the code was cleaned up, duplication removed, and value
  calculations minimised. In particular, instead of four loops to generate the
  front and back faces, two of the loops were put into a method, and adapted to
  generate either face according to the z coordinate argument.
- types --- the data types were changed to use OpenGL types (for example, from
  `float` to `GLfloat`).
- winding --- winding had to be corrected.
- adaption --- the relevant code was modified to create a star instead of a cog,
  and extracted to place into the `StarMesh`. Variable names were modified for
  consistency with the current codebase, and `const` specified as appropriate.

== `glo/scene/animators/orb.*`

I asked DeekSeek to generate code for the Bezier curves. I have experience in
using Bezier curves with computer art programs, but no prior knowledge of how
they work. Therefore, apart from fitting the code into the animator, the Bezier
curve calculation is fully generated by DeepSeek.

== `scene/graphs/light_properties.h`

The Ogre3d wiki @PointLightAttenuation was used for liner and quadratic values
for various light ranges.

I asked DeepSeek to extrapolate linear and quadratic values from the data for
ranges that I felt more desirable.

== `scene/torch.*`

I had a discussion with DeekSeek to find suitable properties for an "adventure
torch". This included colours for the plastic filters, and a colour for a bare
incandescent bulb. I considered the beam width and attenuation suggestions too
large, so asked for pen light properties and used those values instead.

== `textures/*.png`

I made several prompts and refinements to Gemini AI @GoogleGemini to generate
images for the sphere (`animals.png`), cube (`letters-cube.png`), and star
(`sun.png`). Each image was requested separately (i.e. two images for the
sphere, six for the cube, and two for the star). These images were modified as
needed using Inkscape and Gimp, and added to appropriate texture wraps.

== `textures/skybox/*.jpg`

I sourced DDS image files from
#link("https://spacedock.info/mod/926/Pood\%27s\%20Milky\%20Way\%20Skybox") and
converted them to JPEG files using Gimp.

== `glo/scene/emitters/trail.*`

I was already intending to drop the particles, and reduce their size, over time.
The idea of fading them came from reading about particles in the Learn OpenGL
guide, where I also learned of the GL\_ONE blend function. I thought randomising
the colour a little would also add some "sparkle" and asked DeepSeek to give me
a range of individual RGB floats that added together would create a yellow-gold
colour.

I was struggling with the particles turning invisible at certain camera angles.
I reread the Learn OpenGL Guide sections for both particles and instancing. I
learned about Billboarding --- I had missed its importance previously. I
couldn't see where the guide covered this technique in detail, so used DeepSeek
to help me understand the concept.

I ended up with a combination of instancing and particles. Because I had been
reading both sections from the Learn OpenGL Guide, I thought that my
implementation might be wrong, and that I should be using one method or the
other (the two sections are separated by 239 pages in the PDF). I searched the
internet, and realised that this is considered the best approach. I could have
used just particles, as I am only rendering circles, but by using instancing I
have more flexibility should I choose to, for example, add a texture or change
the shape.

== `shaders/blur.frag`

I originally took the code from the Learn OpenGL guide. It is a fixed algorithm,
but I thought the original code a little messy and difficult to maintain. So I
tidied it up by adding appropriate variable usage to make the process clearer,
and added comments, all so I could clearly understand the algorithm myself ---
which as it became clear, is very simple.

The original weights that I used also come from the Learn OpenGL guide. I did
some investigation into different Gaussian approaches. From what I learned, the
single-pass approach produces similar quality results, but is less efficient
(sampling $N^2$ pixels for each pass, compared to $2N$ pixels for the two-pass
approach ($4N$ total)).

I also learned that there are, for example, 5-Tap Kernel and Optimised 5-Tap
Kernel sets of weights --- the optimised versions use computationally cheaper
numbers, with a negligible drop in quality.

I experimented with 3-, 5-, 7-, and 9-Tap weights, which I asked DeepSeek for. I
felt the 9-Tap weights offered a significant quality improvement which
outweighed the extra computational cost over the alternatives that I tried.

However, I was keen to allow the amount of bloom to be adjusted via user input,
and so I rewrote the code entirely to remove the horizontal/vertical duplication
(by introducing the direction variable), and worked around the nature of GLSL's
array handling by introducing switch statements. This sacrifices a little
performance for improved flexibility and maintainability.

== `shaders/bright_pass.frag`

I found the luminance `vec3` in the Learn OpenGL guide, but did some research
into its meaning and original source. It originates from the HDTV Recommendation
ITU-R BT.709\-6 standard for HDTV @HDTV.

The Learn OpenGL Guide provides the Reinhard tone mapping technique. I wanted to
know what else was available, and found that ACES and Uncharted2 are both
popular techniques. These can all be cycled through using the F6 key.

== Models

I purchased the isometric bedroom model @bedroom from cgtrader @cgtrader. The
cradle and its textures @cradle also come from cgtrader. The golf ball and its
textures @golfBall came as a free download from Sketchfab @sketchfab.

= UML

A simplified UML diagram has been made for the source code. It can be found in
the document `assignment-02-uml.pdf`.

#bibliography("refs.bib", title: "References", style: "ieee")
]

