# Qbert Gui Design Notes
March 1st, 2016

## Contents
- Application Abstraction
- Event Routing and Handling
- Layout and Draw
- Attributes and Delegates
- Fundamental QWidgets
- Tabbed and dockable widgets
- Scripted GUI (angel script)

## Application Abstraction
The idea of `QApplication` is to abstract away the underlying win32 API details as much as possible, as well as:
- Stop OS from rendering standard title bar and border
- Mimic standard title bar and border behaviors(move/resize) by responding to OS hit test event(with the help of `QWindow`)
- Support modal tick(QWindow still gets ticked when win32 message loop goes modal)
- Translate win32 messages into Qbert events and route them 
- Recursively layout and draw QWidgets from the top level array of QWindows 

## Event Routing and Handling
Each widget's position is defined relatively to its parent.

> The top level widget(namely a `QWindow`) always takes up the entire client rect of the native win32 window.

### Widget Path
A widget path is a vertical slice of the widget tree.

`QApplication` keeps track of these special paths:
- ClickedPath
    - The path clicked down by mouse
    - Updated when a mouse button is down
    - Used to differentiate valid click, drag/drop, and invalid click(user canceled by moving the cursor away before releasing mouse button)
- HoveredPath
    - The path under mouse curosr
    - Updated when mouse is moving
    - Used to detect when the mouse enters/leaves certain widgets
- FocusedPath
    - The path with keyboard focus
    - Exclusively receives key board event
- CapturedPath(TODO)
    - Used when PIE is needed

> To generate the clicked/hovered path, we recursively traverse the widget tree to transfrom the cursor position into to the current widget's parent space, where a point-rectangle hit test is performed.

### How events are fired
Events are fired synchronizely, there's no event queue. Firing an event is just equivalent to calling a function.

Events are fired down the path, in a similiar way of how tunnel down events are triggered. Each widget in the path gets a chance to handle the event and determine whether or not to propagate it furthur down.

Bubbling up is not supported for now. 

**How to cover this case:** a widget wants to respond to mouse down event only if the mouse down is on itself **and not** on its children widgets.
- With only tunnel down events, the widget cannot tell the difference between a mouse down on itself and a mouse down on its children
- With tunnel down + bubble up events, the widget gets a chance(in the bubbling up MouseDown event) to respond when the mouse down is on its children, but still, it cannot identify the mouse down event happens on itself
- A straight-forward but less elegant method is to allow access the MouseDownPath from `onMouseDown` handler to determine whether or not this is the last widget in the path.

### How to determine which widget is clicked when overlapping happens
For the moment only overlapped top level windows are correctly handled when clicked. 

> The `QApplication` stores all top level `QWindow`s in a back to front ordered array, where the topmost/activated window is always at the array's end. And when another window is about to be activated, we simply erase it from the array(which effectively moves its right-side windows towards the left of the array), then append it to the end of the array.

> And when a mouse click happens on the overlapping area of multiple top level windows, we select the window of the greatest index in the top level array.

This solution is hacky and does not work when overlapped widgets are not direct siblings. 

To correctly pick up the topmost/activated widget among a bunch of overlapped ones, a dynamic layer hierachy must be maintained:

- The easiest to think of is to assign a unique layer id to each widget. But this is a bad idea because a unique layer id generates a unique render batch, which essentially disables the batching.
- Another solution is to divide non-leaf widgets(panels) into two categories
    + panels with no overlapped children
        * given a panel of layer i
        * all its direct children are on a layer of i + 1
        * its direct children must not overlapped with each other(i.e., user cannot move them freely)
        * examples are vertical/horizontal layout panels
    + panels with overlapped children
        * given a panel of layer i
        * its direct children can be on layers of i + 1, i + 2, ...
        * its direct children are movable by the user, but are restrained to the boundary of their parent widget, so we don't have to worry about them overlapping with any non-siblings
        * if a widget does need to overlap with arbitrary widgets, then it must be treated as a floating widget
        * examples are blueprint editor and in-game overlay UI elements
    + floating widget
        * is essentially a standalone top level QWindow with its own backing OS window
        * is dockable

## Layout and Draw
There are two fundamental layout panels: `Vertical Layout` and `Horizontal Layout`.

A `QWidget` also defines two layout properties: `WrapContent` and `MatchParent`, which are appliable to both width and height.

Complex UI structures can be constructed by combining these layout panels and properties.

The above design is implemented in a three-pass processing.

> pass 2 and 3 are actually merged into one in inplementation, but we keep them seperated here for the sake of clarification

### Pass 1: Compute minimal sizes
Post-order traverse to ask each widget to compute its minimal size based on multiple factors
- A leaf widget who can directly determine its size
- A non-leaf widget who has a fixed size
- A non-leaf widget who has a limit of how small it can be
- A non-leaf widget who is folded
- A non-leaf widget's children's minimal sizes, and how its children are to be arranged

We have to use post-order traversal to make sure that when a QWidget's `computeMinimalSize()` method is called, all its children have their minimal sizes already computed.

The traverse is done by `QApplication`. All QWidgets must implement this method.

### Pass 2: Layout children 
Pre-order traverse to let each widget layout its children. Each layout panel must implement this method.

The primary task of pass 2 is to construct a rectangle for each child, given a rectangle allocated by one's parent.

Besides that, layer increment and `ToRoot` accumulation are also handled here:

- a widget's position is defined relatively to its parent, this is fine for hit test since we always transform cursor position to one's parent space before hit test. But rendering requires `visual element`s to be positioned in the root space(i.e. relative to the top-left corner of win32 client area), thus we have to pass and accumulate a `ToRoot` transformation along the traversal.
- as for layer increment, for panels with non-overlapped children, it is a `i + 1` mechanism; for panels with overlapped children, it depends on the specific implementation.

### Pass 3 Draw

- Leaf widgets append `visual element`s to the output draw buffer
- Non-leaf widgets recursively call Draw() on its children, and optionally append border/background elements to the output draw buffer
- All QWidgets update their positions and sizes here

