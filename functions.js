function plot() {
    // Get sin arg and polar ratio
    const sinArg = document.getElementById('range').value;
    const polarRatio = document.getElementById('polar-range').value;

    // Graphics axis names
    const layout = { 
        xaxis: { title: 'X-axis' }, 
        yaxis: { title: 'Y-axis' }, 
    }; 

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

    trace = { 
        x: xPolarValues, 
        y: yPolarValues, 
        type: 'scatter', 
        mode: 'lines', 
    }; 

    Plotly.newPlot(polarGraph, [trace], layout); 

    document.getElementsByClassName("main-svg")[3].style.cssText = "background: rgba(0, 0, 0, 0);";
    document.getElementById("polar-label").textContent = "Частота намотки: " + polarRatio;
}