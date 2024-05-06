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

function secondPlot() {
    // Get polar ratio
    const polarRatio = document.getElementById('second-polar-range').value;

    // Plot base sin
    const graph = document.getElementById('second-graph'); 
    graph.innerHTML = ''; 

    var xValues = [];
    for (let i = 0; i <= 6 * Math.PI; i += 0.01) {
        xValues.push(i);
    }

    var yValues = [];
    for (let i = 0; i <= 6 * Math.PI; i += 0.01) {
        yValues.push(Math.sin(2 * i) + Math.sin(3 * i) + Math.sin(5 * i));
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
    document.getElementsByClassName("main-svg")[9].style.cssText = "background: rgba(0, 0, 0, 0);";

    // Plot polar graphic
    const polarGraph = document.getElementById('second-polar-graph'); 
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
        xaxis: { title: 'X-axis', range: [-3.1, 3.1] }, 
        yaxis: { title: 'Y-axis', range: [-3.1, 3.1]}
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

    document.getElementsByClassName("main-svg")[12].style.cssText = "background: rgba(0, 0, 0, 0);";
    document.getElementById("second-polar-label").textContent = "Частота намотки: " + polarRatio;


    // Mass graphic
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

    const massGraph = document.getElementById('second-mass-graph'); 
    massGraph.innerHTML = ''; 

    Plotly.newPlot(massGraph, [graphTrace, massTrace], layout); 

    document.getElementsByClassName("main-svg")[15].style.cssText = "background: rgba(0, 0, 0, 0);";
}