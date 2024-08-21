<?php
// Thông tin kết nối tới MySQL database
$servername = "localhost";
$username = "root";
$password = "H&ptiot2024";
$dbname = "sensor";

// Tạo kết nối
$conn = new mysqli($servername, $username, $password, $dbname);

// Kiểm tra kết nối
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Xử lý yêu cầu để thêm dữ liệu vào bảng sensor_data
if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    if (isset($_POST['temperature']) && isset($_POST['humidityAir']) && isset($_POST['time'])) {
        $temperature = $conn->real_escape_string($_POST['temperature']);
        $humidityAir = $conn->real_escape_string($_POST['humidityAir']);
        $time = $conn->real_escape_string($_POST['time']);

        $sql = "INSERT INTO sensor_data (nhiet_do, do_am, created_at) 
                VALUES ('$temperature', '$humidityAir', '$time')";

        if ($conn->query($sql) === TRUE) {
            echo "New record created successfully";
        } else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }
    } else {
        echo "Missing temperature, humidityAir, or time data.";
    }
}

// Xử lý yêu cầu AJAX để lấy giá trị mới nhất
if (isset($_GET['latest'])) {
    $sql_latest = "SELECT nhiet_do, do_am, created_at FROM sensor_data ORDER BY created_at DESC LIMIT 1";
    $result_latest = $conn->query($sql_latest);

    if ($result_latest->num_rows > 0) {
        $latest_data = $result_latest->fetch_assoc();
        echo json_encode($latest_data);
    } else {
        echo json_encode(["error" => "No data found"]);
    }
    exit();
}
?>

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Sensor Dashboard</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f0f0f0;
            color: #333;
            margin: 0;
            padding: 0;
        }

        .dashboard {
            max-width: 600px;
            margin: 50px auto;
            padding: 20px;
            background-color: #fff;
            border-radius: 10px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
            text-align: center;
        }

        .dashboard h2 {
            margin-bottom: 20px;
            color: #4CAF50;
        }

        .data p {
            font-size: 1.2rem;
            margin: 10px 0;
        }

        table {
            width: 100%;
            margin-top: 20px;
            border-collapse: collapse;
            background-color: #fafafa;
        }

        table, th, td {
            border: 1px solid #ddd;
            padding: 8px;
            text-align: center;
        }

        th {
            background-color: #4CAF50;
            color: white;
        }

        tr:hover {
            background-color: #f1f1f1;
        }
    </style>
</head>

<body>
    <div class="dashboard">
        <h2>Sensor Dashboard</h2>
        <div class="data">
            <p><strong>Temperature:</strong> <span id="temperature">Loading...</span> °C</p>
            <p><strong>Humidity:</strong> <span id="humidity">Loading...</span> %</p>
            <p><strong>Last updated:</strong> <span id="timestamp">Loading...</span></p>
        </div>
    </div>

    <h2 style="text-align: center;">Data from sensor</h2>
    <table style="margin: 0 auto;">
        <tr>
            <th>ID</th>
            <th>Temperature</th>
            <th>Humidity</th>
            <th>Timestamp</th>
        </tr>
        <?php
        $sql_show = "SELECT id, nhiet_do, do_am, created_at FROM sensor_data ORDER BY created_at DESC";
        $result = $conn->query($sql_show);

        if ($result->num_rows > 0) {
            while ($row = $result->fetch_assoc()) {
                echo "<tr>
                        <td>" . $row['id'] . "</td>
                        <td>" . $row['nhiet_do'] . "</td>
                        <td>" . $row['do_am'] . "</td>
                        <td>" . $row['created_at'] . "</td>
                      </tr>";
            }
        } else {
            echo "<tr><td colspan='4'>No data found in the database.</td></tr>";
        }
        ?>
    </table>

    <script>
        function fetchLatestData() {
            fetch(window.location.href + '?latest=1')
                .then(response => response.json())
                .then(data => {
                    if (!data.error) {
                        document.getElementById('temperature').textContent = data.nhiet_do;
                        document.getElementById('humidity').textContent = data.do_am;
                        document.getElementById('timestamp').textContent = data.created_at;
                    } else {
                        console.error("No data found");
                    }
                })
                .catch(error => console.error('Error fetching data:', error));
        }

        setInterval(fetchLatestData, 5000);

        function timedRefresh(timeoutPeriod) {
            setTimeout(function () {
                location.reload(true);
            }, timeoutPeriod);
        }

        window.onload = function () {
            fetchLatestData();
            timedRefresh(5000);
        };
    </script>
</body>

</html>

<?php
$conn->close();
?>
