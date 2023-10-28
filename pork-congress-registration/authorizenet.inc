<?php
# Authorize.net payment

// AUTHORIZE.NET TRANSACTION
$DEBUGGING					= 1;				# Display additional information to track down problems
$TESTING						= 1;				# Set the testing flag so that transactions are not live
$ERROR_RETRIES			= 2;				# Number of transactions to post if soft errors occur

// BEGIN TEST ACCOUNT
//$auth_net_login_id	= "5h7FLr3Pj";
//$auth_net_tran_key	= "9jM849PUjA33fZtM";
//$auth_net_url				= "https://test.authorize.net/gateway/transact.dll";
// END TEST ACCOUNT


#  Uncomment the lines ABOVE for test accounts or BELOW for live merchant accounts

// BEGIN LIVE ACCOUNT
$auth_net_login_id	= "5h7FLr3Pj";
$auth_net_tran_key	= "9jM849PUjA33fZtM";
$auth_net_url				= "https://secure.authorize.net/gateway/transact.dll";
// END LIVE ACCOUNT

$authnet_values				= array
(
	"x_login"				=> $auth_net_login_id,
	"x_version"			=> "3.1",
	"x_delim_char"			=> "|",
	"x_delim_data"			=> "TRUE",
	"x_url"				=> "FALSE",
	"x_type"				=> "AUTH_CAPTURE",
	"x_method"			=> "CC",
	"x_tran_key"			=> $auth_net_tran_key,
	"x_relay_response"		=> "FALSE",
	"x_card_num"			=> $ccnumber,
	//"x_card_num"			=> "4242424242424242",
	"x_exp_date"			=> $ccexpiration,
	"x_amount"			=> $total,

	// BILLING INFORMATION
	"x_first_name"	=> $fname,
	"x_last_name"		=> $lname,
	"x_company"			=> $company,
	"x_address"			=> $address,
	"x_city"				=> $city,
	"x_state"				=> $state,
	"x_zip"					=> $zip,
	"x_country"			=> $country,
	"x_phone"				=> $phone,
	"x_email"				=> $email,

	// SHIPPING INFORMATION
	"x_ship_to_first_name"	=> $fname,
	"x_ship_to_last_name"	=> $lname,
	"x_ship_to_company"		=> $company,
	"x_ship_to_address"		=> $address,
	"x_ship_to_city"			=> $city,
	"x_ship_to_state"			=> $state,
	"x_ship_to_zip"				=> $zip,
	"x_ship_to_country"		=> $country,

	"x_invoice_num"				=> $orderid,
	"x_cust_id"						=> $email,
	"x_description"				=> "MN Pork Congress Website Order",
	"x_duplicate_window"	=> "600",
);

/*
echo "<pre>";
print_r($authnet_values);
echo "</pre>";
*/

$fields = "";
foreach( $authnet_values as $key => $value ) $fields .= "$key=" . urlencode( $value ) . "&";

//$ch = curl_init("https://test.authorize.net/gateway/transact.dll");
###  Uncomment the line ABOVE for test accounts or BELOW for live merchant accounts
$ch = curl_init("https://secure.authorize.net/gateway/transact.dll");
curl_setopt($ch, CURLOPT_HEADER, 0); // set to 0 to eliminate header info from response
curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1); // Returns response data instead of TRUE(1)
curl_setopt($ch, CURLOPT_POSTFIELDS, rtrim( $fields, "& " )); // use HTTP POST to send form data
### curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE); // uncomment this line if you get no gateway response. ###
$resp = curl_exec($ch); //execute post and get results
curl_close ($ch);

$text = $resp;
$tok = strtok($text,"|");
while(!($tok === FALSE))
{
	$tok = strtok("|");
}

$howMany = substr_count($resp, "|");

$text = $resp;
$h = substr_count($text, "|");
$h++;

$p = strpos($text, "|");
if($p === false) // note: three equal signs
{
	$error[]="<div class=error>Error processing credit card information. Please go back and try again.</div>";
}
else
{
	$p++;

	//  We found the x_delim_char and accounted for it . . . now do something with it

	//  get one portion of the response at a time
	$pstr = substr($text, 0, $p);

	//  this prepares the text and returns one value of the submitted
	//  and processed name/value pairs at a time
	//  for AIM-specific interpretations of the responses
	//  please consult the AIM Guide and look up
	//  the section called Gateway Response API
	$pstr_trimmed = substr($pstr, 0, -1); // removes "|" at the end

	if($pstr_trimmed=="")
	{
		$pstr_trimmed="NO VALUE RETURNED";
	}

	$fval="";
	if($pstr_trimmed=="1")
	{
		$fval="Approved";
		$_SESSION["paymentaccepted"] = "yes";
	}
	elseif($pstr_trimmed=="2")
	{
		$error[] = "<div class=error>Credit Card Declined.</div>";

		$fval="Declined";
	}
	elseif($pstr_trimmed=="3")
	{
		$fval="Error";
		$error[] = "<div class=error>There was an error with the credit card information you have provided us.</div>";
	}
}
?>