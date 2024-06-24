// Max sinus argument value
const SinMaxArg = 3.8;

// Config with plotly graphics settings
const PlotlyConfig = {
    displayModeBar: false,
    doubleClick: false
};

// Function for plot first part of sin graphs
function plotFirst() {
    // Plot first sin cartesian
    var SinArgs = [];
    var SinValues = [];

    for (let i = 0; i <= SinMaxArg * Math.PI; i += 0.01) {
        SinArgs.push(i);
        SinValues.push(Math.sin(i));
    }

    var trace = { 
        x: SinArgs, 
        y: SinValues, 
        type: 'scatter', 
        mode: 'lines',
        hoverinfo: 'none'
    }; 

    var layout = { 
        xaxis: { title: 'X-axis', range: [0, SinMaxArg * Math.PI + 0.5], fixedrange: true}, 
        yaxis: { title: 'Y-axis', fixedrange: true},
    }; 

    Plotly.newPlot('cartesian-sin-graph-first', [trace], layout, PlotlyConfig);

    // Plot first sin polar
    var xPolarValues = [];
    var yPolarValues = [];

    for (let i = 0; i < SinArgs.length; i++) {
        xPolarValues.push(SinValues[i] * Math.sin(SinArgs[i]));
        yPolarValues.push(SinValues[i] * Math.cos(SinArgs[i]));
    }

    var layout = {
        autosize: false,
        width: 500,
        height: 500,
        xaxis: { title: 'X-axis', range: [-1.1, 1.1], fixedrange: true}, 
        yaxis: { title: 'Y-axis', range: [-1.1, 1.1], fixedrange: true},
    }; 

    var graphTrace = {
        x: xPolarValues,
        y: yPolarValues,
        type: 'scatter',
        mode: 'lines',
        hoverinfo: 'none'
    }; 

    Plotly.newPlot('polar-sin-graph-first', [graphTrace], layout, PlotlyConfig);

    removeBackground();
}

// Function for plot second sin graph in polar
function plotSecond() {
    // Calculate standart sin values
    var SinArgs = [];
    var SinValues = [];

    for (let i = 0; i <= SinMaxArg * Math.PI; i += 0.01) {
        SinArgs.push(i);
        SinValues.push(Math.sin(i));
    }

    const polarRatio = document.getElementById('polar-sin-range-second').value;
    var xPolarValues = [];
    var yPolarValues = [];

    for (let i = 0; i < SinArgs.length; i++) {
        xPolarValues.push(SinValues[i] * Math.sin(SinArgs[i] * polarRatio));
        yPolarValues.push(SinValues[i] * Math.cos(SinArgs[i] * polarRatio));
    }

    var layout = {
        autosize: false,
        width: 500,
        height: 500,
        xaxis: { title: 'X-axis', range: [-1.1, 1.1], fixedrange: true}, 
        yaxis: { title: 'Y-axis', range: [-1.1, 1.1], fixedrange: true},
    }; 

    var graphTrace = {
        x: xPolarValues,
        y: yPolarValues,
        type: 'scatter',
        mode: 'lines',
        hoverinfo: 'none'
    }; 

    Plotly.newPlot('polar-sin-graph-second', [graphTrace], layout, PlotlyConfig);
    removeBackground();
}

// Function to call on page load and resize
function pageLoad() {
    // Plot first graphic with sinus in cartesian
    plotFirst();

    // Plot second graphic with sinus in polar with 'polar-sin-range-second' range value
    plotSecond();
}

function pageResize() {
    // Plot first graphic with sinus in cartesian
    plotFirst();

    // Plot second graphic with sinus in polar with 'polar-sin-range-second' range value
    plotSecond();
}

function removeBackground() {
    const containers = document.getElementsByClassName("main-svg");
    
    containers.forEach(element => {
        element.style.cssText = "background: rgba(0, 0, 0, 0);";
    });
}

function plot() {
    // Get sin arg and polar ratio
    const sinArg = document.getElementById('range').value;
    const polarRatio = document.getElementById('polar-range').value;

    // Plot base sin
    const graph = document.getElementById('graph'); 
    graph.innerHTML = ''; 

    var xValues = [];
    for (let i = 0; i <= 6 * Math.PI; i += 0.01) {
        xValues.push(i);
    }

    var yValues = [];
    for (let i = 0; i <= 6 * Math.PI; i += 0.01) {
        yValues.push(Math.sin(sinArg * i));
    } 

    var trace = { 
        x: xValues, 
        y: yValues, 
        type: 'scatter', 
        mode: 'lines', 
    }; 

    var layout = { 
        xaxis: { title: 'X-axis', range: [-0.5, 6 * Math.PI + 0.5] }, 
        yaxis: { title: 'Y-axis'},
    }; 

    Plotly.newPlot(graph, [trace], layout); 
    document.getElementsByClassName("main-svg")[0].style.cssText = "background: rgba(0, 0, 0, 0);";
    document.getElementById("graph-label").textContent = "Частота синусоиды: " + sinArg;

    // Plot polar graphic
    const polarGraph = document.getElementById('polar-graph'); 
    polarGraph.innerHTML = ''; 

    var xPolarValues = [];
    var yPolarValues = [];

    for (let i = 0; i < xValues.length; i++) {
        xPolarValues.push(yValues[i] * Math.sin(xValues[i] * polarRatio));
        yPolarValues.push(yValues[i] * Math.cos(xValues[i] * polarRatio));
    }

    layout = { 
        autosize: false,
        width: 500,
        height: 500,
        xaxis: { title: 'X-axis', range: [-1.1, 1.1] }, 
        yaxis: { title: 'Y-axis', range: [-1.1, 1.1]}
    }; 

    var massX = 0;
    var massY = 0;
    for (let i = 0; i < xPolarValues.length; i++) {
        massX += xPolarValues[i];
        massY += yPolarValues[i];
    }

    var graphTrace = { 
        x: xPolarValues, 
        y: yPolarValues, 
        type: 'scatter', 
        mode: 'lines', 
    }; 

    massX /= xPolarValues.length;
    massY /= xPolarValues.length;

    var massTrace = {
        x: [massX], 
        y: [massY], 
        type: 'scatter', 
        mode: 'marker',
        marker: {
            size: 10,
        }
    };

    Plotly.newPlot(polarGraph, [graphTrace, massTrace], layout); 

    document.getElementsByClassName("main-svg")[3].style.cssText = "background: rgba(0, 0, 0, 0);";
    document.getElementById("polar-label").textContent = "Частота намотки: " + polarRatio;


    // Mass graphic
    var xValues = [];
    for (let i = 0; i <= 6 * Math.PI; i += 0.01) {
        xValues.push(i);
    }

    var yValues = [];
    for (let i = 0; i <= 6 * Math.PI; i += 0.01) {
        yValues.push(Math.sin(sinArg * i));
    }

    var x = [];
    var y = [];

    for (let i = 1; i <= 6; i += 0.01) {
        var xPolarValues = [];
        var yPolarValues = [];

        for (let k = 0; k < xValues.length; k++) {
            xPolarValues.push(yValues[k] * Math.sin(xValues[k] * i));
            yPolarValues.push(yValues[k] * Math.cos(xValues[k] * i));
        }

        var mass = 0;
        for (let i = 0; i < xPolarValues.length; i++) {
            mass += xPolarValues[i];
        }
        mass /= xPolarValues.length;

        x.push(i);
        y.push(mass);
    }

    var layout = { 
        xaxis: { title: 'X-axis', range: [0.8, 6.2] }, 
        yaxis: { title: 'Y-axis'}
    };

    console.log(x);
    console.log(y);

    var graphTrace = {
        x: x, 
        y: y, 
        type: 'scatter', 
        mode: 'lines', 
    }; 

    var massTrace = {
        x: [polarRatio], 
        y: [massX], 
        type: 'scatter', 
        mode: 'marker',
        marker: {
            size: 10,
        }
    };

    const massGraph = document.getElementById('mass-graph'); 
    massGraph.innerHTML = ''; 

    Plotly.newPlot(massGraph, [graphTrace, massTrace], layout); 

    document.getElementsByClassName("main-svg")[6].style.cssText = "background: rgba(0, 0, 0, 0);";
}