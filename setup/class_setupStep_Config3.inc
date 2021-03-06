<?php

/*
  This code is part of FusionDirectory (http://www.fusiondirectory.org/)
  Copyright (C) 2007  Fabian Hickert
  Copyright (C) 2011  FusionDirectory

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
*/


class Step_Config3 extends setup_step
{
  var $header_image = "images/setup/server.png";

  var $errorlvl   = false;

  var $login_attributes = array("uid"       => "uid",
                                "mail"      => "mail",
                                "uid,mail"  => "both");
  var $optional = array(
              "login_attribute" => "uid",
              "strict_units" => false,
              "list_summary" => true,
              "forceglobals" => true,
              "forcessl" => false,
              "ldapstats" => false,
              "warnssl" => true,
              "user_filter_cookie" => true,
              "debuglevel" => 0,
              "session_lifetime" => 1800,
              "compile" =>  SPOOL_DIR,
              "max_ldap_query_time" => "5.0",
              "max_ldap_query_time_active" => FALSE,

              "mailQueueScriptPath_active" => FALSE,
              "notifydir_active" => FALSE,
              "ppd_path_active" => FALSE,
              "kioskpath_active" => FALSE,
              "gosaSupportURI_active" => FALSE,
              "kioskpath_active" => FALSE,

              "enableCopyPaste" => FALSE,
              "logging"         => TRUE,
              "uniq_identifier" => "entryCSN",
              "uniq_identifier_active" => TRUE,

              "sudoou_active" => FALSE,

              "noprimarygroup"  => FALSE);

  var $attributes = array("errorlvl");

  function Step_Config3()
  {
    $this->update_strings();
  }


  function update_strings()
  {
    $this->s_title      = _("FusionDirectory settings 3/3");
    $this->s_title_long = _("FusionDirectory settings 3/3");
    $this->s_info       = _("Tweak some FusionDirectory core behaviour");
  }


  function execute()
  {
    global $BASE_DIR;
    $cv = $this->parent->captured_values;
    /*$this->optional['sudoou'] = preg_replace("/%BASE%/",$cv['base'],$this->optional['sudoou']);*/
    $smarty = get_smarty();
    $smarty->assign("bool",array(FALSE => _("No"), TRUE => _("Yes")));
    $smarty->assign("uniq_identifiers",array( "entryCSN" => "entryCSN",
                                              "contextCSN"=>"contextCSN" ));

    $smarty->assign("logging",$this->optional['logging']);

    $smarty->assign("optional", reverse_html_entities($this->optional));
    foreach($this->attributes as $attr){
      $smarty->assign($attr,reverse_html_entities($this->$attr));
    }
    $smarty->assign("login_attributes",$this->login_attributes);
    return $smarty->fetch("$BASE_DIR/setup/setup_config3.tpl");
  }


  function save_object()
  {
    if(isset($_POST['step6a_posted'])){

      /* Get attributes */
      foreach($this->attributes as $attr){
        if(isset($_POST[$attr])){
          $this->$attr = validate(get_post($attr));
        }
      }

      foreach(array("login_attribute","list_summary","strict_units","noprimarygroup","forceglobals","forcessl","ldapstats","user_filter_cookie","warnssl","compile","debuglevel","session_lifetime","enableCopyPaste","logging") as $name){
        if(isset($_POST[$name])){
          $this->optional[$name] = get_post($name);
        }
      }

      $this->optional['ppd_path_active'] = FALSE;

      if(isset($_POST['max_ldap_query_time_active'])){
        $this->optional['max_ldap_query_time_active'] = TRUE;
        if(isset($_POST['max_ldap_query_time'])){
          $this->optional['max_ldap_query_time'] = get_post('max_ldap_query_time');
        }
      }else{
        $this->optional['max_ldap_query_time_active'] = FALSE;
      }

      $this->optional['mailQueueScriptPath_active'] = FALSE;

      $this->optional['kioskpath_active'] = FALSE;

      $this->optional['sudoou_active'] = FALSE;

      $this->optional['notifydir_active'] = FALSE;

      $this->optional['gosaSupportURI_active'] = FALSE;

      if(isset($_POST['enableCopyPaste'])){
        $this->optional['enableCopyPaste'] = get_post('enableCopyPaste');
      }

      /* Get logging */
      $this->optional['logging'] =  isset($_POST['logging']);

      if(isset($_POST['uniq_identifier_active'])){
        $this->optional['uniq_identifier_active'] = TRUE;
        if(isset($_POST['uniq_identifier'])){
          $this->optional['uniq_identifier'] = get_post('uniq_identifier');
        }
      }else{
        $this->optional['uniq_identifier_active'] = FALSE;
      }
    }

    $tmp = $this->check();
    if(count($tmp) == 0){
      $this->is_completed = TRUE;
    }else{
      $this->is_completed = FALSE;
    }
  }


  function check()
  {
    $message = array();

    if(!is_numeric( $this->optional['session_lifetime'])){
      $message[] = _("Session lifetime must be a numeric value!");
    }

    if(!is_numeric( $this->optional['max_ldap_query_time'])){
      $message[] = _("Maximum LDAP query time must be a numeric value!");
    }

    return($message);
  }


  /* Attributes that are interesting for configuration generation */
  function get_attributes()
  {
    $tmp = setup_step::get_attributes();
    foreach(array("optional") as $attr){
      $tmp[$attr]= $this->$attr;
    }
    return($tmp);
  }

}

// vim:tabstop=2:expandtab:shiftwidth=2:filetype=php:syntax:ruler:
?>
