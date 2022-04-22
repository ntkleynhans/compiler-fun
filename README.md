# Compiler fun

Based off LLVM tutorial - [https://llvm.org/docs/tutorial/index.html](https://llvm.org/docs/tutorial/index.html)

# Language definition 

// define entry point
@entry = main

comment: //
end expression: ;
assignment: =
definition: :=
types: string, int, float, double, bool, struct
address: @ (pointer)
operators: +,-,/,\*,&,| 


## Function Prototype

@Namespace::funcName (argOne : Type, argTwo : Type) ReturnType {

    return varA, varB, ...
}

@func(var type)
@func(var @type)

## Comment

// comment

## Definition

y : int; // declare - auto initialize y = 0
x : int := 1;
a : float := 1.0;
m, n : float := 2.0;

## Pointer

// struct Ptr { 
//  start = 0x0
//  size = 4
// } 

x : int;
y : @int;
y : @int := x;

y @+ 4; // address increment

y @= 4; // dereference assignment \*y = 4
// y = &0xFF -> &0xFF = 4

x @= y;

## Array

x : int[4];
x : int[] := 1,2,3,4;
x : int[] := 1..10; 

## Struct

X : struct {
    a : string;
    b : @string;
}

x : X = { "hello", @"world" };
y : @X;

y = x;
y.a
y.@a

### Hidden
    \_address : uint[];
    \_size : int;

## Assignment

x : int := 4
y : int;

y = x // value

## Operations

x = x + 1
x++
x += 1


x | w


## Scope


## Iter



## For

for X {

}


