

<?php
$input_file = "file.txt";
$output_file = "assembly.s";
$out_code = NULL;
$output = NULL;
$in_code = NULL;
$retval = NULL;
if (isset($_POST['code'])){
  $in_code = $_POST['code'];
  file_put_contents($input_file, $in_code);
  exec("code_generator");
  system("taskkill /F /IM \"out.exe\" /T");
  system("gcc assembly.s -o out");
  exec("out", $output, $retval);
  $output = implode("\n", $output);
  $out_code = file_get_contents($output_file);
}

echo "<form method = 'post' name ='index' action = 'index.php'>
        <label for='code'></label>
        <textarea id='code' name='code' rows='25' cols='80'>".$in_code."</textarea>
        <br>
        <button type='submit' class='compile_btn' >Compile and run</button>
    </form>
    <textarea id='compiled_code' name='compiled_code' rows='25' cols='80' >".$out_code."</textarea>
    <textarea id='output' name='output' rows='7' cols='25' >Output:\n".$output."\n\nReturn value:\n".$retval." </textarea>
    <style>label,
    textarea {
      font-size: 0.8rem;
      letter-spacing: 1px;
    }
    
    textarea {
    position: relative;
    left:4%;
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
        left:56%;
        bottom: 75.5%;
    }
    #output{
        position: relative;
        left:2.5%;
        bottom: 47.5%;
    }
    button{
        position: relative;
        left:46.6%;
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
    </style>"
?>