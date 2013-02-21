var palette = new Rickshaw.Color.Palette();
var SERIES_MAX = 100;
var driver_version = "", nvml_version = "";
var devices = [];

var graphs = {
    clock:       { series: [] },
    fan:         { series: [] },
    memory:      { series: [] },
    power:       { series: [] },
    temperature: { series: [] },
    utilization: { series: [] }
};

function init(data) {
    driver_version = data.driver_version;
    nvml_version = data.nvml_version;

    $('#host').append(data.host);
    $('#info #driver').append(driver_version);
    $('#info #nvml').append(nvml_version);
    $('#info #gpus').append(data.devices.length);

    for(var i in data.devices) {
        devices[i] = devices[i] || { color: palette.color(),
                                     features: {}
                                   };

        for(var j in data.devices[i].features) {
            var feature = data.devices[i].features[j];

            devices[i].features[feature] = true;
        }
    }
}

function update(data) {
    if(data == null) {
        $('#connection').text("Interrupted");
        $('#connection').addClass("interrupt");
        return;
    } else {
        $('#connection').text("OK");
        $('#connection').removeClass("interrupt");
    }

    for(var i in data.devices) {
        var dev = data.devices[i];

        for(var key in graphs) {
            var graph = graphs[key];

            // Just skip if the device doesn't support this operation.
            if(dev[key] == null) continue;

            var color = devices[i].color;

            var val = 0;
            if(typeof graph.getData != 'undefined')
                val = graph.getData(dev);

            if(graph.series[i] == null) {
                graph.series[i] = {
                    name: dev.name + ":" + dev.index,
                    data: [{x: data.time, y: val}],
                    color: color
                };
            } else {
                graph.series[i].data.push({ x: data.time, y: val});
            }

            while(graph.series[i].data.length >= SERIES_MAX) {
                graph.series[i].data.splice(0,1);
            }
        }
    }

    for(var i in graphs) {
        if(graphs[i].graph != null) {
            graphs[i].graph.update();
            graphs[i].graph.update();
        }
    }
}

function ajaxInit() {
    init($.parseJSON($.ajax({
        type: 'GET',
        dataType: 'json',
        url:"ajax/init",
        async: false,
        data: {},
    }).responseText));
}


function ajaxUpdate() {
    update($.parseJSON($.ajax({
        type: 'GET',
        dataType: 'json',
        url:"ajax/update",
        async: false,
        data: {},
    }).responseText));
}

function addGraph(key, obj) {
    if(typeof obj === 'undefined') {
        obj = {};
    }

    var fmt = obj.tickFormat || function(y) { return y; };

    if(graphs[key].series.length == 0) {
        console.log('No GPUs support "' + key + '" disabling...');
        $('#graphs-list').append('<li class="disabled"><del>' + key +
                                 '</del></li>');
        return;
    }

    var name = obj.name || key;

    $('#body').append('<div class="chart_container" id="' + key +'">' +
                      '<span class="name">' + name + '<br/></span>' +
                      '<div class="chart"></div>' +
                      // '<div class="legend"><span class="name">' + name +
                      // '</span></div>' +
                      '</div>');

    var graph = new Rickshaw.Graph({
        element: document.querySelector('#' + key + " .chart"),
        width: 350,
        height: 150,
        renderer: 'line',
        series: graphs[key].series
    });

    if(obj.min != null) graph.min = obj.min;
    if(obj.max != null) graph.max = obj.max;

    graphs[key].key = key;
    graphs[key].getData = obj.getData || function(dev) { return dev[key]; };

    var hoverDetail = new Rickshaw.Graph.HoverDetail( {
        graph: graph
    });

    var time = new Rickshaw.Fixtures.Time();

    var x_axis = new Rickshaw.Graph.Axis.Time({
        graph: graph,
        timeUnit: time.unit('minute')
    });

    var y_axis = new Rickshaw.Graph.Axis.Y( {
        graph: graph,
        tickFormat: fmt,
        element: document.querySelector('#' + key + ' .y_axis'),
    });

    // var legend = new Rickshaw.Graph.Legend( {
    //     element: document.querySelector('#' + key + ' .legend'),
    //     graph: graph
    // } );

    // var highlighter = new Rickshaw.Graph.Behavior.Series.Highlight({
    //     graph: graph,
    //     legend: legend
    // });

    // var shelving = new Rickshaw.Graph.Behavior.Series.Toggle( {
    //     graph: graph,
    //     legend: legend
    // });

    $('#graphs-list').append('<li id="' + key + '" class="drag"><a href="#' +
                             key +'">' + name + '</a></li>');

    graph.render();
    graphs[key].graph = graph;
}

ajaxInit();

// Initialize data
ajaxUpdate();

function fmtPercent(val) { return val + "%"; }

addGraph('temperature',
         { tickFormat: function(y) { return y + "C"; } } );

addGraph('power',
        { tickFormat: function(y) { return y + "mW"; } } );

addGraph('memory',
         { tickFormat: fmtPercent, min: 0, max: 100,
           getData: function(dev) {
               return +dev.memory.used / +dev.memory.total * 100;
           }});

addGraph('fan',
         { tickFormat: fmtPercent, min: 0, max: 100,
           getData: function(dev) { return +dev.fan; }
         });

// TODO: Add other clock speeds in here as well
addGraph('clock',
         { name: 'graphics clock',
           tickFormat: function(y) { return y + "MHz"; },
           getData: function(dev) { return dev.clock.graphics; }});

// TODO: Add GPU utilization as well
addGraph('utilization',
         { name: 'memory utilization', min: 0, max: 100,
           tickFormat: fmtPercent,
           getData: function(dev) { return +dev.utilization.memory; }});

setInterval(ajaxUpdate, 1000);

// Update our charts to match the newly sorted list
$('.sortable').sortable({items: '.drag'}).bind('sortupdate', function() {
    var container = $('#body');
    var graphs = container.children();
    container.empty();

    var origPos = {};
    for(var i in graphs) {
        origPos[graphs[i].id] = graphs[i];
    }

    $('#graphs-list .drag').each(function(i, li) {
        var id = li.getAttribute('id');
        container.append(origPos[id]);
    });
});
