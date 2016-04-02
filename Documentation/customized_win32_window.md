# WindowStyle
```cpp
    windowStyle = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
```
- WS_POPUP The windows is a pop-up window. This style cannot be used with the WS_CHILD style.
- WS_CLIPCHILDREN Excludes the area occupied by child windows when drawing occurs within the parent window. This style is used when creating the parent window.
- WS_CLIPSIBLINGS Clips child windows relative to each other; that is, when a particular child window receives a WM_PAINT message, the WS_CLIPSIBLINGS style clips all other overlapping child windows out of the region of the child window to be updated. If WS_CLIPSIBLINGS is not specified and child windows overlap, it is possible, when drawing within the client area of a child window, to draw within the client area of a neighboring child window.

``` cpp
    windowStyle |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
    windowStyle |= WS_MAXIMIZEBOX;
    windowStyle |= WS_MINIMIZEBOX;
    windowStyle |= WS_THICKFRAME;
```
- WS_OVERLAPPED The window is an overlapped window. An overlapped window has a title bar and a border.

- WS_BORDER The window has a thin-line border.

- WS_CAPTION The window has a title bar (includes the WS_BORDER style).

- **WS_SYSMENU** The window has a window menu on its title bar. The WS_CAPTION style must also be specified.

- **WS_MAXIMIZEBOX** The window has a maximize button. Cannot be combined with the WS_EX_CONTEXTHELP style. The WS_SYSMENU style must also be specified.

- **WS_MINIMIZEBOX** The window has a minimize button. Cannot be combined with the WS_EX_CONTEXTHELP style. The WS_SYSMENU style must also be specified.

- **WS_THICKFRAME** The window has a sizing border.
 
 
 


### SetWindowRgn() 
sets the window region of a window. The window region determines the area within the window where the system permits drawing. The system does not display any portion of a window that lies outside of the window region.
