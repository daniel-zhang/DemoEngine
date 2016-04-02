# The Subtle Behaviors of D3D Shader Compiler
Feb 13, 2016

### Un-referenced variables are dropped

A cbuffer like this gets completely dropped 
```java
    cbuffer SomeBuffer 
    {
        float4 gUnusedVar0;
        float4 gUnusedVar1;
    };
```
There seems to be no compiler flag to stop this behavior, the drop happens even with debug enabled and optimization skipped. 

### But if parent buffer contains used vars, unused variables then gets kept 

`gUnusedVar` will survive the compilation
```java
    cbuffer SomeBuffer 
    {
        float4 gUnusedVar;
        float4 gUsedVar;
    };
```
This also applies to special cbuffers like `$Globals`. 

If the shader source code contains only unused global vars, they get dropped during the compilation, and there is no `$Globals` cbuffer in the compiled bytecode. 

However, if there is at least one global variable gets referenced, then the rest global vars will also be kept, no matter whether they are referenced or not. 

### The above observation applies well until one comes across with what they called "Class Instance" variables

```java
interface IBar
{
    float4 foo();
};

class BarImple1 : IBar
{
    float4 foo();
};

class BarImple2 : IBar
{
    float4 mLocalVar;
    float4 foo();
};

// This gets into special cbuffer $ThisPointer
IBar iBar;

cbuffer InterfaceImplementations
{
    BarImple1 bar1;
    BarImple2 bar2;
}

```
First of all, all class instance variables like `bar1` and `bar2` are not referenced in the shader code. They are supposed to be set from the application side. 

But they still get kept in the compilation, unless they themselves are 'empty'. An 'empty' class instance is one who does not define any local member varialbes.

> e.g. class `BarImple1` is empty, and is virtually 'invisible' to the compiler.

### "Interface Instances" are stored in a special cbuffer called `$ThisPointer`

One way to reflect cbuffers is to iterate bound resources for resource of type `SIT_CBUFFER`. But this does not work for `$ThisPointer`, which is a cbuffer but is not a bound resource.
