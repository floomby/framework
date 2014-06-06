#!/usr/bin/env ruby

# A glorified preprocessor/templating thingy

require 'erb'
require 'ostruct'

class Quote
    def initialize parent, str
        parent.send 'write_through', str
        parent.send 'idle'
    end
end

class Print
    def initialize parent, str
        parent.send 'write_through', str.gsub(/@(((\\@)|([^@]))+)@/){ |expr| ret = (eval unescape expr); "#{ret}".chomp! }
        parent.send 'idle'
    end
    def unescape str
        ((str.gsub /^@/, '').gsub /@$/, '').gsub /\\@/, '@'
    end
end

class Namespace
    def initialize parent, name
        @parent = parent
        @name = name
        @state = 'Idle'
        @parent.send 'write_through', "namespace #{@name} {"
    end
    def parse_line line
        if @state == 'Idle'
            if /^[ ]*end[ ]*$/.match line
                @parent.send 'write_through', "} // end namespace #{@name}"
                @parent.send 'idle'
            else
                a = /^[ ]*([^ ]+) (.*)[ ]*$/.match line
                abort "syntax error #{line}" unless a
                @state = a[1]
                @obj = (Kernel.const_get a[1]).new self, a[2]
            end
        else
            @obj.send 'parse_line', line
        end
    end
    def write_through what
        @parent.send 'write_through', what
    end
    def idle
        @state = 'Idle'
    end
end

class Header
    @@template =    "#ifndef <%= name.upcase %>_H_INCLUDED\n"   +
                    "#define <%= name.upcase %>_H_INCLUDED\n\n" +
                    "<%= body %>\n\n"                           +
                    "#endif//<%= name.upcase %>_H_INCLUDED\n"
    def initialize parent, name
        @parent = parent
        @name = name
        @body = ''
        @state = 'Idle'
        @file = "#{name}.h"
    end
    def parse_line line, action
        @action = action
        if @state == 'Idle'
            a = /^[ ]*end(?:[ ]+([^ ]+))?[ ]*$/.match line
            if a
                if /./.match a[1]
                    @file = a[1]
                end
                write
                @parent.send 'idle'
            else
                a = /^[ ]*([^ ]+) (.*)[ ]*$/.match line
                abort "syntax error #{line}" unless a
                @state = a[1]
                @obj = (Kernel.const_get a[1]).new self, a[2]
            end
        else
            @obj.send 'parse_line', line
        end
    end
    def write
        case @action
        when 'generate'
            namespace = OpenStruct.new name: @name, body: @body
            renderer = ERB.new @@template
            file = File.open @file, "w"
            file.write renderer.result namespace.instance_eval { binding }
        when 'clean'
            File.delete @file
        else
            abort "unsuported action: #{@action}"
        end
    end
    def idle
        @state = 'Idle'
    end
    def write_through what
        @body << what << "\n"
    end
end

class DrcvParser
    def initialize file
        @data = IO.read file
    end
    def run action
        @action = action
        @state = 'Idle'
        @line = 0
        @data.each_line { |line| parse_line line }
    end
    def parse_line line
        line.chomp!
        @line += 1
        if @state == 'Idle'
            a = /^[ ]*([^ ]+) ([^ ]+)[ ]*$/.match line
            abort "syntax error on line #{@line}: #{line}" unless a
            @state = a[1]
            @obj = (Kernel.const_get a[1]).new self, a[2]
        else
            @obj.send 'parse_line', line, @action
        end
    end
    def idle
        @state = 'Idle'
    end
end

action = ARGV.shift
ARGV.each { |a| drcvp = DrcvParser.new a; drcvp.run action }
