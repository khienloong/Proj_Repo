var express = require("express");
var router = express.Router();
var mysql = require("mysql");

//Pooling mysql connections
const pool = mysql.createPool({
	host: "logo2.c8v6vdi8dcei.us-east-2.rds.amazonaws.com",
	user: "logo2",
	password: "logologo",
	database: "IoT_Project",
});

//Set the image time
router.get('/time_get', (req, res, next) => {
	var today = new Date();
	today.setHours(today.getHours() - 6);

	const json = JSON.stringify(today);
	res.send(json);
});

router.get("/set-points_get", (req, res, next) => {

	pool.getConnection((err, conn) => {
	if(err) throw err;
	var sql = "SELECT * FROM set_points";

	conn.query(sql, (err, row) => {
	//	conn.release();
		if(err) throw err;

		const json = JSON.stringify(row);
		res.send(json);
		conn.release();
		});

	});
}); 


router.get("/current_temp_get", (req, res, next) => {

	pool.getConnection((err, conn) => {
	if(err) throw err;
	var sql = "SELECT * FROM current_temp";

	conn.query(sql, (err, row) => {
	//	conn.release();
		if(err) throw err;

		const json = JSON.stringify(row);
		res.send(json);
		conn.release();
		});

	});
});


router.get("/Action_get", (req, res, next) => {

	pool.getConnection((err, conn) => {
	if(err) throw err;
	var sql = "SELECT * FROM Action";

	conn.query(sql, (err, row) => {
	//	conn.release();
		if(err) throw err;

		const json = JSON.stringify(row);
		res.send(json);
		conn.release();
		});

	});
});


//Miguel's help, do for each table
router.post("/set-point_post", (req, res, next) => {

	var time1 = req.body.time1;
	var temp1 = req.body.temp1;
	var temp2 = req.body.temp2;

	pool.getConnection((err, conn) => {
	if(err) throw err;

		var sql = "UPDATE set_points SET time = " + time1 + ", temp1 = " + temp1 + ", temp2 = " + temp2;
		conn.query(sql, (err, result) => {
		if(err) res.send("\r\n Couldn't update set_points\r\n");
		res.send("data updated");
		conn.release();
		});
	
	    if(err) throw err;

	    var sql = "SELECT * FROM set_points"; //selects all data in table
	   // var sql = "SELECT * FROM set_points WHERE id = 1"; //gets a row from the table
	   // var sql = "SELECT time, temp1, temp2 FROM set_points"; //Gives specific colums for whole table
	   // var sql = "SELECT time, FROM set_points WHERE id = 1"; //Grabs specific column for a specific row

	    conn.query(sql, (err, result) =>
	    { 
		    if(err) throw err;
		    var rows = JSON.stringify(result);
		    res.send(rows);

	     });
    });
}); 

//Do one of these for each of the tables
router.post("/current_temp_post", (req, res, next) =>{
	
	var time = req.body.time;
	var temp = req.body.temp;
	var status = req.body.status;

	pool.getConnection((err, conn)=> {
	if(err) throw err;
		
	var sql = "INSERT INTO current_temp(temp, status, time) VALUES("+time+","+temp+","+status+")";
		conn.query(sql, (err, result) => {
		if(err) throw err;
		res.send('Record inserted');
		conn.release();
		});	
});
});

router.post("/set_points_post", (req, res, next) =>{
	
	var temp1 = req.body.temp1;
	var temp2 = req.body.temp2;
	var time1 = req.body.time1;
	var time2 = req.body.time2;
	var time3 = req.body.time3;

	pool.getConnection((err, conn) => {
	if(err) throw err;
		
		var sql = "INSERT INTO set_points(temp1, temp2, time1, time2, time3) VALUES("+temp1+","+temp2+","+time1+","+time2+","+time3+")";
		
		conn.query(sql, (err, result) => {
		if(err) throw err;
		res.send('Record inserted');
		conn.release();
		});
	});
});

router.post("/Action_post", (req, res, next) =>{
	
	var time = req.body.time;
	var status = req.body.status;

 
	pool.getConnection((err, conn) => {
	if(err) throw err;
		
		var sql = "INSERT INTO Action (time, status) VALUES ("+time+","+status+")";
		
		conn.query(sql, (err, result) => {
		if(err) throw err;
		res.send('Record inserted');
		conn.release();
		});
	});
});



module.exports = router;
