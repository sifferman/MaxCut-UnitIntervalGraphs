#!/usr/bin/env wolframscript


CUT[ x_, X_, y_, Y_, z_, Z_ ] := x*( X - x ) + y*( Y - y ) + z*( Z - z ) + x*y + ( X - x )*( Y - y ) + z*y + ( Z - z )*( Y - y )


MAXCUT[ S1_, S2_, S3_ ] := Maximize[
    {
        CUT[ n1, S1, n2, S2, n3, S3 ],

        0 <= n1 <= S1 &&
        0 <= n2 <= S2 &&
        0 <= n3 <= S3 &&

        (* Element[ a | b | c | X | Y | Z, Integers ] && *)

        1 <= S1 &&
        1 <= S2 &&
        1 <= S3
    },

    { n1, n2, n3 }
]
Print[ MAXCUT[S1, S2, S3] ]
