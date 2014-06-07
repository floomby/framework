#!/usr/bin/env ruby

require 'socket'
require 'readline'

require 'inline'

class NonBlockingReadline
    inline :C do |builder|
        builder.add_link_flags "-ledit -lruby191"
        builder.include '<editline/readline.h>'
src = <<END
    struct data_t {
        char *ret;
        char *prompt;
    };
    
    VALUE readline_intern(void *arg)
    {
        struct data_t data = (struct data_t *)arg;
        
        data->readline(data->);
        return Qnil;
    }
END
        builder.prefix src
src = <<END
    VALUE rb_readline(void){
        struct data_t data;
        
        data.ret = NULL;
        data.prompt = "> ";
        
        rb_thread_blocking_region(readline_intern, &data, NULL, NULL);
        return rb_str_new2(data.ret);
    }
END
        builder.c_singleton src, method_name: 'readline'
    end
end # class BetterReadline


class Session
    def initialize client
        @client = client
        @recv_thr = Thread.new { recver }
    end

    def self.recver
        loop do
            msg = @client.recv 4096
            @buf << msg
        end
    end
    
    def self.sender what
        @client.write what
    end
    
#    def self.syncr
#        ret = @buf
#        @buf = ''
#        ret
#    end
end




class HandServ

    def initialize
        @server = TCPServer.new 1234
        @sessions = []
        @idx = 0
    end

    def handler
    
        loop do
            @buf << "here"
            client = @server.accept
            @sessions << (Session.new client)
            port, ip = Socket.unpack_sockaddr_in client.getpeername
            @buf << "new conection from #{ip} on port #{port}"
        end
    end

    def self.go
        @thr = Thread.new { handler }
    end

    def write
        idx = 0
        @sessions.each do |s|
            
            
            
        end
    end
    
    def self.sender what
        puts @buf
        #@sessions[@idx].sender what
    end
end

s = HandServ
s.go

while buf = (NonBlockingReadline.readline)
    s.sender buf.chomp
end
