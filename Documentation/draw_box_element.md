# General process for drawing a box element

```cpp
//     ___LeftMargin    ___RightMargin
//    /                /
//  +--+-------------+--+
//  |  |c1           |c2| ___TopMargin
//  +--o-------------o--+
//  |  |             |  |
//  |  |c3           |c4|
//  +--o-------------o--+
//  |  |             |  | ___BottomMargin
//  +--+-------------+--+

//    ___LeftX          ___RightPaddedX
//   /   _LeftPaddedX  /  ____RightX
//  /   /             /  /
//  +--+-------------+--+ <---TopY
//  |  |c1           |c2| 
//  +--o-------------o--+ <---TopPaddedY
//  |  |             |  |
//  |  |c3           |c4|
//  +--o-------------o--+ <---BottomPaddedY
//  |  |             |  | 
//  +--+-------------+--+ <---BottomY
```

- The input element looks like this
```cpp
    class Element {
    public:
        vector2 pos;
        vector2 size;
        float scale;

        float angle;
        vector2 rotation_point;

        color vertex_color;
        Texture texture;

        vector2 uv_start; // either be a mono texture
        vector2 uv_size;  // or a texture atlas

        uint32_t tiling_rule // enum {u_wrap, v_wrap, both, none };

        vector4 margin;
    };
```

- Determine uv address mode as in: `D3D11_TEXTURE_ADDRESS_CLAMP` or `D3D11_TEXTURE_ADDRESS_WRAP`
```cpp
    typedef struct D3D11_SAMPLER_DESC {
        ...
        D3D11_TEXTURE_ADDRESS_MODE AddressU;
        D3D11_TEXTURE_ADDRESS_MODE AddressV;
        D3D11_TEXTURE_ADDRESS_MODE AddressW;
        ...
        }   D3D11_SAMPLER_DESC;
```    

- Compute tiling
```cpp
    tiling_x = size.x / texture.width or 1.f if no tiling;
    tiling_y = size.y / texture.height or 1.f if no tiling;

    vector2 tiling = vector2(tiling_x, tiling_y);
```

- Rotation 
```cpp
    vector4 rot = angle != 0.0f ? vector4( angle, rotation_point.x, rotation_point.y, 0 ) : vector4();
```
- If all four margins are 0, fall back to a plain quad

- Otherwise, figure out the margin dimensions(left, top, right, bottom) in uv space[0.0f, 1.0f].
> element.margin is defined as percentages that describe how much area each margin takes up
> e.g. the left margin in uv space is given by `element.uv_start.x + element.margin.left * element.uv_size.x`

- Figure out how many texels each margin is taking up



