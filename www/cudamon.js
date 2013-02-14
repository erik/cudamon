var series = [];

// Initialize
$.getJSON("ajax/update", function(data) {
    alert(data);
});

function update() {
    d3.json("ajax/update", function(data) {
        console.log(this);
        for(dev in data['devices']) {
            series += {name:dev.name, color:'#FF00FF', data:[dev.temperature]};
        }
    });
}

var graph = new Rickshaw.Graph( {
    element: document.getElementById("chart"),
    width: 500,
    height: 200,
    renderer: 'line',
    series: series
});

graph.render();

var hoverDetail = new Rickshaw.Graph.HoverDetail( {
    graph: graph
});

var legend = new Rickshaw.Graph.Legend( {
    graph: graph,
    element: document.getElementById('legend')

} );

var shelving = new Rickshaw.Graph.Behavior.Series.Toggle( {
    graph: graph,
    legend: legend
});

var axes = new Rickshaw.Graph.Axis.Time( {
    graph: graph
});

axes.render();
