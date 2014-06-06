#!/usr/bin/env ruby

# A decent (not recursive) parser to
# parse the directive files
# probably super buggy but if you write good
# directive files it should work just fine

require 'json'
require 'erb'
require 'ostruct'

class Declstruct
    def initialize parent, name
        abort "Declstruct not implemented"
    end
end

class Constdata
    def initialize parent, name
        abort "Constdata not implemented"
    end
end

class Header
    
#    @@declaration = '<%= type %> <%= token %>;'
#    @@assingment  = '<%= token %> = <%= value %>;'
#    @@constdecl   = 'const <%= type %> <%= token > = <%= value %>;'
    
    def initialize parent, name
        @parent = parent
        @name = name
        @body = ''
        @state = 'Idle'
    end
    
    def parse_line line
        
        if @state == 'Idle'
            if /^[ ]?+end[ ]?+$/.match line
                write
                @parent.send 'idle'
            else
                a = /^[ ]?+([^ ]+) ([^ ]+)[ ]?+$/.match line
                raise 'syntax error' unless !a
                @state = a[1]
                @obj = (Kernel.const_get a[1]).new a[2]
            end
        else
            @obj.send 'parse_line', line
        end
        
    end
    
    def write
        
        namespace = OpenStruct.new name: @name, body: @body
        template = IO.read 'templates/header.h.erb'
        renderer = ERB.new template
        file = File.open "#{@name}.h", "w"
        file.write renderer.result namespace.instance_eval { binding }
        
    end
    
end


class Nasm
    def initialize parent, name
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

# 

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
        @line += 1
        
        if @state == 'Idle'
            a = /^[ ]?+([^ ]+) ([^ ]+)[ ]?+$/.match line
            abort "syntax error on line #{@line}: #{line}" unless a
            @state = a[1]
            @obj = (Kernel.const_get a[1]).new self, a[2]
        else
            @obj.send 'parse_line', line
        end
    end
    

    def idle
        @state = 'Idle'
    end
        
end


drcvp = DrcvParser.new ARGV.first
drcvp.run
