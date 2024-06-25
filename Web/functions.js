// Config with plotly graphics settings
const PlotlyConfig = {
    displayModeBar: false,
    doubleClick: false,
};

// Max sinus argument value
const SinMaxArg = 3.8;

// Graphics size
const GraphSize = 700;

// Precalculated values to  graphics
var xCartesian = [];
var yCartesian = [];
var xPolar = [];
var yPolar = [];

// Precalculations
for (let i = 0; i <= SinMaxArg * Math.PI; i += 0.01) {
    xCartesian.push(i);
    yCartesian.push(Math.sin(i));
    xPolar.push(yCartesian.at(-1) * Math.sin(i));
    yPolar.push(yCartesian.at(-1) * Math.cos(i));
}

// Function to draw graphic in cartesian coordinate system
function plotGraphCartesian(graphName, x, y) {
    var trace = { 
        x: x, 
        y: y, 
        type: 'scatter', 
        mode: 'lines',
        hoverinfo: 'none'
    }; 

    var layout = { 
        xaxis: { title: 'X-axis', range: [0, SinMaxArg * Math.PI + 0.5], fixedrange: true}, 
        yaxis: { title: 'Y-axis', fixedrange: true},
    }; 

    Plotly.newPlot(graphName, [trace], layout, PlotlyConfig);
}

// Function to draw graphic in polar coordinate system
function plotGraphPolar(graphName, x, y) {
    var layout = {
        autosize: false,
        width: GraphSize,
        height: GraphSize,
        xaxis: { title: 'X-axis', range: [-1.1, 1.1], fixedrange: true}, 
        yaxis: { title: 'Y-axis', range: [-1.1, 1.1], fixedrange: true},
    }; 

    var graphTrace = {
        x: x,
        y: y,
        type: 'scatter',
        mode: 'lines',
        hoverinfo: 'none'
    }; 

    Plotly.newPlot(graphName, [graphTrace], layout, PlotlyConfig);
}

// Function to call on page load and resize
function drawStatic() {
    plotGraphCartesian("static1", xCartesian, yCartesian);
    
    drawCartesianToPolarGraph();

    plotGraphPolar("static2", xPolar, yPolar);

    drawWrapToPolarGraph();

    // Remove background
    removeBackground();
}

// Function to remove background from graphics
function removeBackground() {
    const containers = document.getElementsByClassName("main-svg");
    
    containers.forEach(element => {
        element.style.cssText = "background: rgba(0, 0, 0, 0);";
    });
}


// Function to draw cartesian to polar graph
function drawCartesianToPolarGraph() {
    var circleTrace = {
        x: xPolar,
        y: yPolar,
        type: 'scatter',
        mode: 'lines',
        hoverinfo: 'none'
    }; 

    const pointPos = 12;
    var lineTrace1 = {
        x: [xPolar[Math.round(xPolar.length / pointPos)], xPolar[Math.round(xPolar.length / pointPos)]],
        y: [yPolar[Math.round(xPolar.length / pointPos)], 0],
        type: 'scatter',
        mode: 'lines',
        hoverinfo: 'none',
        line: {
            dash: 'dot',
            width: 4
          }
    };

    var lineTrace2 = {
        x: [0, xPolar[Math.round(xPolar.length / pointPos)]],
        y: [yPolar[Math.round(xPolar.length / pointPos)], yPolar[Math.round(xPolar.length / pointPos)]],
        type: 'scatter',
        mode: 'lines',
        hoverinfo: 'none',
        line: {
            dash: 'dot',
            width: 4
          },
    };

    var lineTrace3 = {
        x: [0, xPolar[Math.round(xPolar.length / pointPos)]],
        y: [0, yPolar[Math.round(xPolar.length / pointPos)]],
        type: 'scatter',
        mode: 'lines',
        hoverinfo: 'none',
        line: {
            dash: 'dot',
            width: 4
          },
    };

    var layout = {
        autosize: false,
        width: GraphSize,
        height: GraphSize,
        xaxis: { title: 'X-axis', range: [-1.1, 1.1], fixedrange: true}, 
        yaxis: { title: 'Y-axis', range: [-1.1, 1.1], fixedrange: true},
        showlegend: false,
        annotations: [
            {
              x: xPolar[Math.round(xPolar.length / pointPos)] / 2,
              y: yPolar[Math.round(xPolar.length / pointPos)],
              xref: 'x',
              yref: 'y',
              text: 'x = r * cos(fi) = sin(x) * cos(x)',
              showarrow: true,
              arrowhead: 7,
              ax: -60,
              ay: -30
            }, {
                x: xPolar[Math.round(xPolar.length / pointPos)],
                y: yPolar[Math.round(xPolar.length / pointPos)] / 2,
                xref: 'x',
                yref: 'y',
                text: 'x = r * sin(fi) = sin(x) * sin(x)',
                showarrow: true,
                arrowhead: 7,
                ax: 20,
                ay: -100
            }, {
                x: xPolar[Math.round(xPolar.length / pointPos)] / 2,
                y: yPolar[Math.round(xPolar.length / pointPos)] / 2,
                xref: 'x',
                yref: 'y',
                text: 'r = sin(x)',
                showarrow: true,
                arrowhead: 7,
                ax: -120,
                ay: 0
            }, {
                x: 0,
                y: 0,
                xref: 'x',
                yref: 'y',
                text: 'fi = x',
                showarrow: true,
                arrowhead: 7,
                ax: -20,
                ay: -20
            }
          ]
    }; 

    Plotly.newPlot("cartesianToPolar", [circleTrace, lineTrace1, lineTrace2, lineTrace3], layout, PlotlyConfig);
}

function drawWrapToPolarGraph() {
    const wrapRatio = document.getElementById('wrapToPolarRange').value;
    
    const graph = document.getElementById('wrapToPolar'); 
    graph.innerHTML = ''; 

    var xWrap = [];
    var yWrap = [];

    for (let i = 0; i < xCartesian.length; i++) {
        xWrap.push(yCartesian[i] * Math.sin(xCartesian[i] * wrapRatio));
        yWrap.push(yCartesian[i] * Math.cos(xCartesian[i] * wrapRatio));
    }

    var graphTrace = { 
        x: xWrap, 
        y: yWrap, 
        type: 'scatter', 
        mode: 'lines', 
    };
    
    var layout = {
        autosize: false,
        width: GraphSize,
        height: GraphSize,
        xaxis: { title: 'X-axis', range: [-1.1, 1.1], fixedrange: true}, 
        yaxis: { title: 'Y-axis', range: [-1.1, 1.1], fixedrange: true},
    }; 

    document.getElementById("wrapToPolarLabel").textContent = "Частота намотки: " + wrapRatio;
    Plotly.newPlot(graph, [graphTrace], layout, PlotlyConfig);
    removeBackground();
}