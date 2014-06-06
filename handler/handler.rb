#!/usr/bin/env ruby

require 'socket'

server = TCPServer.new 1234

loop do
    client = server.accept
    thr = Thread.new { loop { $stdout.write client.recv 4096 } }
    loop do
        client.send gets.chomp, 0
    end
    Thread.kill thr
end

