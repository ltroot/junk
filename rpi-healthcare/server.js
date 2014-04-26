var http = require('http'),
	url = require('url'),
	fs = require('fs'),
	qs = require('querystring'),
	io = require('socket.io');

var stat;

var server = http.createServer(function (request, response) {
	var path = url.parse(request.url).pathname;

	switch(path) {
		case '/':
			response.writeHead(200, {'Content-Type': 'text/html'});
			response.write('Index');
			response.end();
			break;
		case '/client':
			fs.readFile('./client.html', function (error, data) {
				response.write(data);

				if(request.method == 'POST') {
					var formData = '';

					request.on('data', function (data) {
						formData += data;
					});

					request.on('end', function () {
						var post = qs.parse(formData);
						for(key in post) {
							var command = post[key];
							if(command.match('run')) {
								stat = 'run';
							} else if(command.match('rotate')) {
								stat = 'rotate';
							} else {
								stat = 'Error: command not found';
							}
						}
					});
				}
				response.end();
			});
			break;
		default:
			response.writeHead(404);
			response.write("404 - Page not found.");
			response.end();
			break;
	}
});

server.listen(8080);
var serv_io = io.listen(server);

serv_io.sockets.on('connection', function (socket) {
	socket.emit('updateStatus', {'stat': stat});
});
