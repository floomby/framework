#!/usr/bin/env ruby

# A fsm to parse the directive files
# super buggy but if you write good
# directive files it should work just fine


require 'erb'
require 'ostruct'

class Header
    
    def initialize name
    
        @name = name
        puts "new Header #{name}"
    
    end
    
    def parse_line line
        
        puts "Header got line #{line}"
        
    end
    
    def write
        
        namespace = OpenStruct.new name: @name
        template = IO.read 'templates/header.h.erb'
        renderer = ERB.new template
        file = File.open "#{@name}.h", "w"
        file.write renderer.result namespace.instance_eval { binding }
        
    end
    
end


class Nasm
    def initialize name
        @name = name
        abort "Nasm backend not implemented"
    end
    
    def parse_line line
        abort "Nasm backend not implemented"
    end
    
    def write
        abort "Nasm backend not implemented"
    end
end


class DrcvParser

    def initialize file
        @data = IO.read file
    end

    def run
        @state = 'Idle'
        @line = 0
        @data.each_line { |line| parse_line line }
    end

    
    def parse_line line
        
        line.chomp!
        
        if @state != 'Idle'
            @obj.send 'parse_line', line
        else
            a = /^([^ ]+) ([^ ]+)$/.match line
            if /^end[ ]*/.match line
                @obj.send 'write'
                @state = 'Idle'
            elsif !a
                abort "syntax error on line #{@line}: #{line}"
            else
                @obj = (Kernel.const_get a[1]).new a[2]
            end
        end
        
    end
        
end


drcvp = DrcvParser.new ARGV.first
drcvp.run
