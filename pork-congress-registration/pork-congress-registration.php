<?php
/**
 * Plugin Name: Pork Congress Registration
 * Plugin URI: http://www.mnpork.com
 * Description: Allows members to register for Pork Congress, and staff to modify dates on the form each year.
 * Version: 1.0.0
 * Author: Stephen O. Ogunlana
 * Author URI: http://stephenogunlana.com
 * License: Not to be used by anyone other than Minnesota Pork Producers.  Contact VoyageurWeb with any questions.
 */

defined('ABSPATH') or die("No scripts please!");

add_action( 'admin_menu', 'pork_congress_registration_menu' );

// Adds a menu link in the Dashboard underneath the Settings menu
function pork_congress_registration_menu() {	
	 add_options_page( 'Pork Congress Registration Options', 'Pork Congress Registration', 'manage_options', 'pork-congress-registration', 'pork_congress_registration_options');
	 
	 // Call register settings function
	 add_action( 'admin_init', 'register_pcr_settings' );
}

// Register our settings
function register_pcr_settings() {
	register_setting( 'pcr-settings-group', 'pcr_year' );
	register_setting( 'pcr-settings-group', 'pcr_event_date' );
	register_setting( 'pcr-settings-group', 'pcr_early_date' );
	register_setting( 'pcr-settings-group', 'pcr_event_id' );
	register_setting( 'pcr-settings-group', 'pcr_promo_code' );
}

// Function that draws the Pork Congress Registration settings screen
function pork_congress_registration_options() {
	if( !current_user_can( 'manage_options' ) ) {
		wp_die( __( 'You do not have sufficient permissions to access this page.' ) );
	}
		
	// Admin screen display output
	?>
    <h1><strong>*** All registrations are sent to colleen@mnpork.com,pam@mnpork.com,accounting@mnpork.com and saved to the root folder (/home/mpork/public_html/1798) in a CSV file (porkcongressYEAR.csv)</strong></h1>
	<div class="wrap">
		<p>This options page is used to set the current registration year and dates, as well as the current event ID and promo code.</p>
		<p>You can put a Pork Congress Registration form on any Page using the shortcode: <strong>[pork-congress-registration]</strong></p>
		
		<form method="post" action="options.php">
			<?php settings_fields( 'pcr-settings-group' ); ?>
			<?php do_settings_sections( 'pcr-settings-group' ); ?>
			<div class="pcr_option_label">Year:</div>
			<div class="pcr_option"><input type="text" name="pcr_year" value="<?php echo esc_attr( get_option('pcr_year') ); ?>" /></div>
			<br />
			<div class="pcr_option_label">Event Date:</div>
			<div class="pcr_option"><input type="text" name="pcr_event_date" value="<?php echo esc_attr( get_option('pcr_event_date') ); ?>" /></div>
			<br />
			<div class="pcr_option_label">Early Registration Deadline:</div>
			<div class="pcr_option"><input type="text" name="pcr_early_date" value="<?php echo esc_attr( get_option('pcr_early_date') ); ?>" /></div>
			<br />
			<div class="pcr_option_label">Event ID:</div>
			<div class="pcr_option"><input type="text" name="pcr_event_id" value="<?php echo esc_attr( get_option('pcr_event_id') ); ?>" /></div>
			<br />
			<div class="pcr_option_label">Promo Code:</div>
			<div class="pcr_option"><input type="text" name="pcr_promo_code" value="<?php echo esc_attr( get_option('pcr_promo_code') ); ?>" /></div>
			<br />
			<?php submit_button(); ?>
		</form>
	</div>
	<?php
}

// Creates a shortcode to place into a WYSIWYG on any Page
function pork_congress_registration_func($atts) {
	// Include the pork registration form and logic
	require_once(dirname(__FILE__) .'/registration.inc');
}
// [pork-congress-registration]
add_shortcode( 'pork-congress-registration', 'pork_congress_registration_func' );
?>
