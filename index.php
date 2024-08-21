<?php
// Thông tin kết nối tới MySQL database
$servername = "localhost";
$username = "root"; // Tên đăng nhập MySQL
$password = "H&ptiot2024"; // Mật khẩu MySQL
$dbname = "sensor";  // Tên database

// Tạo kết nối
$conn = new mysqli($servername, $username, $password, $dbname);

// Kiểm tra kết nối
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Xử lý yêu cầu để thêm dữ liệu vào bảng sensor_data
if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    // Kiểm tra xem có nhận được các thông số temperature, humidityAir và time hay không
    if (isset($_POST['temperature']) && isset($_POST['humidityAir']) && isset($_POST['time'])) {
        // Lấy giá trị từ POST request
        $temperature = $_POST['temperature'];
        $humidityAir = $_POST['humidityAir'];
        $time = $_POST['time'];

        // Kiểm tra và lọc dữ liệu đầu vào
        $temperature = $conn->real_escape_string($temperature);
        $humidityAir = $conn->real_escape_string($humidityAir);
        $time = $conn->real_escape_string($time);

        // Câu truy vấn SQL để chèn dữ liệu vào bảng sensor_data
        $sql = "INSERT INTO sensor_data (nhiet_do, do_am, created_at) 
                VALUES ('$temperature', '$humidityAir', '$time')";

        // Thực hiện truy vấn và kiểm tra kết quả
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
    exit(); // Kết thúc script sau khi trả về JSON
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Sensor Dashboard</title>
    <link rel="stylesheet" href="style.css">
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f0f4f8;
            color: #333;
            margin: 0;
            padding: 0;
            display: flex;
            flex-direction: column;
            align-items: center;
        }

        .dashboard {
            background: #ffffff;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
            border-radius: 10px;
            padding: 20px;
            margin: 20px;
            max-width: 600px;
            width: 100%;
            text-align: center;
        }

        .dashboard h2 {
            margin: 0;
            color: #2c3e50;
        }

        .dashboard p {
            margin: 10px 0;
            font-size: 18px;
        }

        .dashboard span {
            font-weight: bold;
        }

        table {
            width: 80%;
            max-width: 800px;
            margin: 20px;
            border-collapse: collapse;
            background: #ffffff;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
            border-radius: 10px;
        }

        th, td {
            padding: 12px;
            text-align: left;
            border-bottom: 1px solid #ddd;
        }

        th {
            background-color: #3498db;
            color: #ffffff;
        }

        tr:nth-child(even) {
            background-color: #f2f2f2;
        }

        tr:hover {
            background-color: #e0e0e0;
        }

        @media (max-width: 768px) {
            table {
                width: 100%;
            }
        }
    </style>
</head>
<body>
    <div class="dashboard">
        <h2>Sensor Dashboard</h2>
        <div>
            <p><strong>Temperature:</strong> <span id="temperature">Loading...</span> °C</p>
            <p><strong>Humidity:</strong> <span id="humidity">Loading...</span> %</p>
            <p><strong>Last updated:</strong> <span id="timestamp">Loading...</span></p>
        </div>
    </div>

    <h2>Data from sensor</h2>
    <table>
        <tr>
            <th>ID</th>
            <th>Temperature</th>
            <th>Humidity</th>
            <th>Timestamp</th>
        </tr>
        <?php
        // Hiển thị dữ liệu từ bảng sensor_data
        $sql_show = "SELECT id, nhiet_do, do_am, created_at FROM sensor_data ORDER BY created_at DESC"; // Lấy toàn bộ dữ liệu
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
            fetch(window.location.href + '?latest=1') // Gọi chính file PHP này để lấy dữ liệu mới nhất
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

        // Gọi hàm fetchLatestData mỗi 5 giây
        setInterval(fetchLatestData, 5000);

        window.onload = function () {
            fetchLatestData();
        };
    </script>
</body>
</html>

<?php
// Đóng kết nối
$conn->close();
?>
