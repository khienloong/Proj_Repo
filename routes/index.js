var express = require("express");
var router = express.Router();
var mysql = require("mysql");

//Pooling mysql connections
const pool = mysql.createPool({
	host: "logo2.c8v6vdi8dcei.us-east-2.rds.amazonaws.com",
	user: "logo2",
	password: "logologo",
	database: "IoT_Project",
})

//Set the image time
router.get('/time', (req, res, next) => {
	var today = new Date();
	today.setHours(today.getHours() - 6);

	const json = JSON.stringify(today);
	res.send(json);
})

router.get("/set-points", (req, res, next) => {

	pool.getConnection((err, conn) => {
	if(err) throw err;
	var sql = "SELECT * FROM set_points";

	conn.query(sql, (err, row) => {
		conn.release();
		if(err) throw err;

		const json = JSON.stringify(row);
		res.send(json);
		})

	})
}) 


router.get("/current_temp", (req, res, next) => {

	pool.getConnection((err, conn) => {
	if(err) throw err;
	var sql = "SELECT * FROM current_temp";

	conn.query(sql, (err, row) => {
		conn.release();
		if(err) throw err;

		const json = JSON.stringify(row);
		res.send(json);
		})

	})
})


router.get("/Action", (req, res, next) => {

	pool.getConnection((err, conn) => {
	if(err) throw err;
	var sql = "SELECT * FROM Action";

	conn.query(sql, (err, row) => {
		conn.release();
		if(err) throw err;

		const json = JSON.stringify(row);
		res.send(json);
		})

	})
})


//router.post('/set_points', (req, res, next) => {

//}

//Miguel's help, do for each table
router.post("/set-point", (req, res, next) => {
   // var conn = mysql.createConnection({
const pool = mysql.createPool({
    connectionLimit : 100,
    host: "logo2.c8v6vdi8dcei.us-east-2.rds.amazonaws.com",
    user: "logo2",
    password: "logologo",
    database: "IoT_Project",
    debug: false 	
  });

	var time1 = req.params.time1;
	var temp1 = req.params.temp1;
	var temp2 = req.params.temp2;

	conn.connect((err) => {
		if(err) throw err;

		var sql = "UPDATE set_points SET time = " + time1 + ", temp1 = " + temp1 + ", temp2 = " + temp2;
		conn.query(sql, (err, result) => {
			if(err) res.send("\r\n Couldn't update set_points\r\n");
			res.send("data updated");
		})

router.get("/set-points", (req, res, next) => {	
   // var conn = mysql.createConnection({
const pool = mysql.createPool({
    connectionLimit : 100,
    host: "logo2.c8v6vdi8dcei.us-east-2.rds.amazonaws.com",
    user: "logo2",
    password: "logologo",
    database: "IoT_Project",
    debug: false 	
  });

    conn.connect((err) => {
	    if(err) throw err;

	    var sql = "SELECT * FROM set_points"; //selects all data in table
	   /*/ var sql = "SELECT * FROM set_points WHERE id = 1"; //gets a row from the table
	    var sql = "SELECT time, temp1, temp2 FROM set_points"; //Gives specific colums for whole table
	    var sql = "SELECT time, FROM set_points WHERE id = 1"; //Grabs specific column for a specific row*/

	    conn.query(sql, (err, result) =>
	    { 
		    if(err) throw err;
		    var rows = JSON.stringify(result);
		    res.send(rows);

	     })
    })
})

//Do one of these for each of the tables
router.post("/current_temp", (req, res, next) =>{
//    var conn = mysql.createConnection({
   
const pool = mysql.createPool({
    connectionLimit : 100,
    host: "logo2.c8v6vdi8dcei.us-east-2.rds.amazonaws.com",
    user: "logo2",
    password: "logologo",
    database: "IoT_Project",
    debug: false 	
  });
	
	var time = req.params.time;
	var temp = req.params.temp;
	var status = req.params.status;

	conn.connect((err) => {
		if(err) throw err;
		
		var sql = "INSERT INTO current_temp(temp, status, time) VALUES("+time+","+temp+","+status+")";
		
		conn.query(sql, (err, result) => {
			if(err) throw err;
			res.send('Record inserted');
		}
	)}
})

router.post("/set_points", (req, res, next) =>{
//    var conn = mysql.createConnection({
 
const pool = mysql.createPool({
    connectionLimit : 100,
    host: "logo2.c8v6vdi8dcei.us-east-2.rds.amazonaws.com",
    user: "logo2",
    password: "logologo",
    database: "IoT_Project",
    debug: false 	
  });
	
	var temp1 = req.params.temp1;
	var temp2 = req.params.temp2;
	var time1 = req.params.time1;
	var time2 = req.params.time2;
	var time3 = req.params.time3;

	conn.connect((err) => {
		if(err) throw err;
		
		var sql = "INSERT INTO set_points(temp1, temp2, time1, time2, time3) VALUES("+temp1+","+temp2+","+time1+","+time2+","+time3+")";
		
		conn.query(sql, (err, result) => {
			if(err) throw err;
			res.send('Record inserted');
		}
	)}
})

router.post("/Action", (req, res, next) =>{
//    var conn = mysql.createConnection({
 
const pool = mysql.createPool({
    connectionLimit : 100,
    host: "logo2.c8v6vdi8dcei.us-east-2.rds.amazonaws.com",
    user: "logo2",
    password: "logologo",
    database: "IoT_Project",
  });
	
	var time = req.params.time;
	var status = req.params.status;

	conn.connect((err) => {
		if(err) throw err;
		
		var sql = "INSERT INTO current_temp(status, time) VALUES("+time+","+status+")";
		
		conn.query(sql, (err, result) => {
			if(err) throw err;
			res.send('Record inserted');
		}
	)}
})



module.exports = router;
