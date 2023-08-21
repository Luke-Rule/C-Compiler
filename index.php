<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>
<body>
  
<form method = 'post' name ='compile' action = 'compile.php'>
        <label for='code'></label>
        <textarea id='code' name='code' rows='30' cols='80'></textarea>
        <br>
        <button type='submit' class='compile_btn' >Compile</button>
    </form>
    <textarea id='compiled_code' name='compiled_code' rows='30' cols='80' ></textarea>
    <style>label,
    textarea {
      font-size: 0.8rem;
      letter-spacing: 1px;
    }
    
    textarea {
    position: relative;
    left:5%;
        bottom: 88.5%;
      display:inline-block;
      background-color: #303030;
      color: #ffffff;
      padding: 10px;
      max-width: 100%;
      line-height: 1.5;
      border-radius: 5px;
      border: 1px solid #ccc;
      box-shadow: 1px 1px 1px #999;
    }
    #compiled_code{
        position: relative;
        left:54%;
        bottom: 642px;
    }
    button{
        position: relative;
        left:47.8%;
        display:inline-block;
        background-color: #303030;
      color: #ffffff;
      padding: 10px;
      max-width: 100%;
      line-height: 1.5;
      border-radius: 5px;
      border: 1px solid #ccc;
      box-shadow: 1px 1px 1px #999;
    }
    
    label {
      display: block;
      margin-bottom: 10px;
    }
    </style>
    
</body>
</html>