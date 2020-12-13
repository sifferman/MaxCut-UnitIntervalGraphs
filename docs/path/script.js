
/* path/script.js */


const clamp = ( min, X, max ) => Math.max( min, Math.min( X, max ) );
const result = document.getElementById( "Result" );


// Give all class elements attribute index
const K = Array.from(document.getElementsByClassName( "twin-class" )).length;

function giveIndiciesToClassElements( classname ) {
    let elements = Array.from(document.getElementsByClassName( classname ));
    for ( let i = 0; i < K; i++ )
        elements[i].setAttribute( "index", ""+(i+1) )
}
giveIndiciesToClassElements( "s-input" );
giveIndiciesToClassElements( "n-display" );
giveIndiciesToClassElements( "n-slider" );
giveIndiciesToClassElements( "plot" );
giveIndiciesToClassElements( "n-maximize" );

// set twin-class-index
let twin_class_indicies = Array.from(document.getElementsByClassName( "twin-class-index" ));
for ( let i = 0; i < twin_class_indicies.length; i++ )
    twin_class_indicies[i].innerHTML = i+1;

// Create arrays of all elements
    // index 0 is unused
var s_input = [0];
var s_value = [0];

var n_display = [0];
var n_slider = [0];
var n_value = [0];
var n_maximize = [0];

var plot = [0];


// fill all arrays with class elements
s_input = s_input.concat( Array.from(document.getElementsByClassName( "s-input" )), [0] );
for ( let i = 1; i <= K; i++ ) s_value.push( parseInt(s_input[i].value) ); s_value.push(0);


n_display = n_value.concat( Array.from(document.getElementsByClassName( "n-display" )), [0] );
n_slider = n_slider.concat( Array.from(document.getElementsByClassName( "n-slider" )) );
for ( let i = 1; i <= K; i++ ) n_value.push( parseInt(n_slider[i].value) ); n_value.push(0);
n_maximize = n_maximize.concat( Array.from(document.getElementsByClassName( "n-maximize" )) );

plot = plot.concat( Array.from(document.getElementsByClassName( "plot" )) );
const WIDTH = plot[1].width;
const HEIGHT = plot[1].height;
for ( let i = 1; i <= K; i++ )
    plot[i] = plot[i].getContext('2d');



// input handlers
function handleSInput( number ) {
    let i = parseInt( number.getAttribute("index") );
    s_value[i] = parseInt( number.value );
    update();
}
function handleNSlider( slider ) {
    let i = parseInt( slider.getAttribute("index") );
    n_value[i] = parseInt( slider.value );
    update();
}
function handleMaximize( button ) {
    let i = parseInt( button.getAttribute("index") );
    n_value[i] = Math.floor( clamp( 0, (
            Math.random() + 
            2*n_value[i+1]-s_value[i+1]
            + 2*n_value[i-1]-s_value[i-1]
            + s_value[i]
        ) / 2 ,
        s_value[i]
    ));
    update();
}
// randomize
function randomize() {
    for ( let i = 1; i <= K; i++ ) {
        s_value[i] = Math.ceil( Math.random() * 100 );
        n_value[i] = Math.floor( Math.random() * ( s_value[i] + 1 ) );
    }
    update();
}
// solve
function solve() {
    let invert = (Math.random() > .5);
    for ( let i = 1; i <= K; i++ ) {
        let max = Math.min(
            s_value[i],
            ( s_value[i-1]+s_value[i]+s_value[i+1] ) / 2
        );
        max = ( Math.random() > .5 ) ? Math.floor(max) : Math.ceil(max);
        max = ( invert ) ? (s_value[i] - max) : max;
        n_value[i] = max;
    }
    update();
}
// counterexample
function proposeCounterexample() {
    for ( let i = 2; i <= K; i++ ) {
        if ( (i-1) % 3 == 2 ) {
            n_value[i] = s_value[i] / 2;
        } else {
            n_value[i] = ( 2*n_value[1] - s_value[1] + s_value[i] ) / 2;

            if ( (i-1) % 6 > 2 )
                n_value[i] = s_value[i] - n_value[i];
        }
        n_value[i] = ( Math.random() > .5 )
                    ? Math.floor( n_value[i]  ) : Math.ceil( n_value[i] );
        n_value[i] = clamp( 0, n_value[i], s_value[i] );
    }
    update();
}




// update
update();

// update functions
function update() {
    updateInputs();
    updateMaximized();
    updateResult();
    draw();
}

function updateInputs() {
    for ( let i = 1; i <= K; i++ ) {
        n_value[i] = clamp( 0, n_value[i], s_value[i] );
        n_slider[i].setAttribute( "max", s_value[i].toString() );
        n_slider[i].value = n_value[i].toString();
        s_input[i].value = s_value[i].toString();
        n_display[i].innerHTML = n_value[i].toString();
    }
}

// will disable button if n_value is within .5 of local max
function updateMaximized() {

    for ( let i = 1; i <= K; i++ ) {
        let local_max = clamp( 0 ,
                (   2*n_value[i+1] - s_value[i+1]
                     + 2*n_value[i-1] - s_value[i-1]
                     + s_value[i]
                ) / 2 ,
                s_value[i]
        );
        
        n_maximize[i].disabled = ( Math.abs( n_value[i] - local_max ) <= .5 );
    }

}

function updateResult() {

    let out = 0;

    for ( let i = 1; i <= K; i++ ) {
        out += n_value[i] * ( s_value[i] - n_value[i] );
    }
    for ( let i = 1; i < K; i++ ) {
        out += n_value[i]*n_value[i+1]
         + ( s_value[i]-n_value[i] ) * ( s_value[i+1]-n_value[i+1] );
    }

    result.innerHTML = out;
}


// draw functions
function draw__down( ctx ) {
    ctx.beginPath();
    ctx.moveTo( 0, 0 );
    ctx.lineTo( WIDTH, HEIGHT );
    ctx.stroke();
}
function draw__hump( ctx ) {
    ctx.beginPath();
    ctx.moveTo( 0, HEIGHT );
    ctx.lineTo( WIDTH/2, HEIGHT/2);
    ctx.stroke();
    ctx.beginPath();
    ctx.moveTo( WIDTH/2, HEIGHT/2);
    ctx.lineTo( WIDTH, HEIGHT);
    ctx.stroke();
}
function draw__up( ctx ) {
    ctx.beginPath();
    ctx.moveTo( 0, HEIGHT );
    ctx.lineTo( WIDTH, 0);
    ctx.stroke();
}



function draw() {
    
    for ( let i = 1; i <= K; i++ ) {
        plot[i].clearRect(0, 0, WIDTH, HEIGHT);

        if ( s_value[i] == 0 )
            continue;

        let L = clamp(
            0, Math.floor( (
                2*n_value[i+1]-s_value[i+1]
                 + 2*n_value[i-1]-s_value[i-1]
                 + s_value[i]
            ) / 2 ),
            s_value[i]
        );

        if ( L <= .5  )
            draw__down( plot[i] );
        else if ( Math.abs(  s_value[i] - L ) <= .5 )
            draw__up( plot[i] );
        else
            draw__hump( plot[i] );
    }

}
