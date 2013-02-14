var palette = new Rickshaw.Color.Palette();

var driver_version = "", nvml_version = "";

var series = {
    temperature: []
};

var graph = null;

function update(data) {
    driver_version = data.driver_version;
    nvml_version = data.nvml_version;

    for(var i in data.devices) {
        var dev = data.devices[i];

        // TODO: get time from server
        var time = (new Date).getTime() / 1000;

        if(series.temperature[i] == null) {
            series.temperature[i] = { name: dev.name,
                                      data: [{x:time,y:dev.temperature}],
                                      color: palette.color()
                                    };
        } else {
            series.temperature[i]['data'].push({x:time, y:dev.temperature});
        }

        while(series.temperature[i].data.length >= 100) {
            series.temperature[i].data.shift();
        }

    }

    graph && graph.update() && graph.render();
}

// Initialize
function callUpdate() {
    update($.parseJSON($.ajax({
        type: 'GET',
        dataType: 'json',
        url:"ajax/update",
        async: false,
        data: {},
    }).responseText));
}

callUpdate();

setInterval(callUpdate, 1000);

graph = new Rickshaw.Graph( {
    element: document.getElementById("chart"),
    width: 540,
    height: 240,
    renderer: 'line',
    series: series.temperature
});

var hoverDetail = new Rickshaw.Graph.HoverDetail( {
    graph: graph
});

var time = new Rickshaw.Fixtures.Time();

var x_axis = new Rickshaw.Graph.Axis.Time({
    graph: graph,
    timeUnit: time.unit('15 second')
});

var y_axis = new Rickshaw.Graph.Axis.Y( {
    graph: graph,
    tickFormat: function(y) { return y + "C"; },
    element: document.getElementById('y_axis'),
});

var legend = new Rickshaw.Graph.Legend( {
        element: document.querySelector('#legend'),
        graph: graph
} );

var highlighter = new Rickshaw.Graph.Behavior.Series.Highlight({
    graph: graph,
    legend: legend
});

graph.render();
