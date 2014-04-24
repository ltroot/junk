var http = require('http'),
	fs = require('fs'),
	qs = require('querystring');

server = http.createServer(function(request, resource) {
	fs.readFile(__dirname + '/client.html', function(error, data){
		resource.write(data);
		
		if(request.method == 'POST') {
			var formData = '';
			
			request.on('data', function(data) {
				formData += data;
			});
			
			request.on('end', function() {
				var post = qs.parse(formData);
				for(key in post) {
					var command = post[key];
					if(command.match('run')) {
						console.log(command);
						// do something
					} else if (command.match('rotate')) {
						console.log(command);
						// do something
					} else {
						console.log('Error: command not found');
					}
				}
				resource.end();
			});
		}
	});
});

server.listen(8080, '127.0.0.1');
