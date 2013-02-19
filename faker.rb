#!/usr/bin/env ruby

# Generate fake JSON data to allow implementing client without access
# to CUDA devices

require 'sinatra'
require 'json'

$devices = [
            { :name => "Device1", :index => 0, :serial => "abcd1",
              :features => ["temperature", "compute", "utilization",
                            "power", "memory", "clock", "name", "fan"]},
            { :name => "Device2", :index => 1, :serial => "abcd2",
              :features => ["temperature", "compute", "utilization",
                            "power", "memory", "clock", "name", "fan"]},
            { :name => "Device3", :index => 2, :serial => "abcd3",
              :features => ["temperature", "compute", "utilization",
                            "power", "memory", "clock", "name", "fan"]},
            { :name => "Device4", :index => 3, :serial => "abcd4",
              :features => ["temperature", "compute", "utilization",
                            "power", "memory", "clock", "name", "fan"]}
           ]

set :public_folder, 'www/'

get '/' do
  send_file File.join(settings.public_folder, 'index.html')
end

get '/ajax/init' do
  JSON.generate({ :devices => $devices,
                  :driver_version => "1234.5",
                  :nvml_version => "5432.1",
                  :host => "foobarbaz"})
end

get '/ajax/update' do
  $devices.each {|dev|
    dev['memory'] = { :free => rand(200),
      :used => rand(1000),
      :total => 4096
    }

    dev['clock'] = {
      :graphics => 500 + rand(100),
      :sm => 500 + rand(100),
      :mem => 500 + rand(100)
    }

    dev['fan'] = 50+rand(10)

    dev['temperature'] = 50 + (rand(20) - 10)
    dev['power'] = 50 + (rand(20) - 10)

    dev['utilization'] = {
      :memory => 50 + rand(50),
      :gpu => 50 + rand(50)
    }
  }

  JSON.generate({ :devices => $devices,
                  :time => Time.now.to_i})
end
