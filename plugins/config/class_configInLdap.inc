<?php
/*
  This code is part of FusionDirectory (http://www.fusiondirectory.org/)
  Copyright (C) 2012 FusionDirectory

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


class configInLdap extends simplePlugin
{
  var $objectclasses  = array("fusionDirectoryConf");
  var $mainTab        = TRUE;

  static function plInfo()
  {
    return array(
      "plShortName"     => _("Configuration"),
      "plDescription"   => _("FusionDirectory configuration"),
      "plSelfModify"    => FALSE,
      "plPriority"      => 1,
      "plSection"       => array("administration"),
      "plCategory"      => array("configuration"),
      "plObjectType"    => array("configuration" => array("name"  => _("FusionDirectory configuration"),
                                                          "filter"  => "objectClass=fusionDirectoryConf")),

      "plProvidedAcls"  => parent::generatePlProvidedAcls(self::getAttributesInfo())
    );
  }

  static function getAttributesInfo ()
  {
    return array(
      'schema_setup' => array(
        'name'  => _('Schema setup'),
        'attrs' => array(
          new BooleanAttribute (
            _('rfc237bis'),
            _('Enables rfc2307bis style groups. You can use member attributes instead of memberUid in this case.'),
            'fdRfc2307bis', FALSE,
            FALSE
          ),
          new BooleanAttribute (
            _('schemaCheck'),
            _('Enables schema checking during login.'),
            'fdSchemaCheck', FALSE,
            FALSE
          ),
        )
      ),
      'look_n_feel' => array(
        'name'  => _('Look n feel'),
        'attrs' => array(
          new SelectAttribute (
            _('Language'), _('Default language to be used if the one asked by the browser is not available'),
            'fdLanguage', FALSE,
            array('en_US')
          ),
          new SelectAttribute (
            _('Theme'), _('Theme to be used'),
            'fdTheme', TRUE,
            self::get_themes(),
            'default'
          ),
          new SelectAttribute (
            _('Timezone'), _('Timezone to be used'),
            'fdTimezone', TRUE,
            array('America/New_York')
          ),
        )
      ),
      'people_and_group' => array(
        'name'  => _('People and group storage'),
        'attrs' => array(
          new SelectAttribute (
            _('People DN attribute'), _('Attribute to use at the beginning of users dn'),
            'fdAccountPrimaryAttribute', TRUE,
            array('cn', 'uid')
          ),
          new BooleanAttribute (
            _('Include personal title in user DN'),
            _('Include personal title in user DN'),
            'fdPersonalTitleInDN'
          ),
          new StringAttribute (
            _('Customized CN'), _('Use a placeholder pattern for generating account cns'),
            'fdAccountRDN', FALSE
          ),
          new StringAttribute (
            _('Next id hook'), _('A script to be called for finding the next free id for users or groups externaly.'),
            'fdNextIdHook', FALSE
          ),
          new StringAttribute (
            _('Automatic UIDs'), _('Automatic way to generate new user ids'),
            'fdIdGenerator', FALSE
          ),
          new BooleanAttribute (
            _('Strict naming policy'),
            _('Enables strict checking of uids and group names'),
            'fdStrictNamingRules', FALSE,
            TRUE
          ),
          new IntAttribute (
            _('GID/UID min id'),
            _('The minimum assignable user or group id to avoid security leaks with uid 0 accounts.'),
            'fdMinId', FALSE,
            0, FALSE
          ),
          new IntAttribute (
            _('Number base for people'),
            _('Where to start looking for a new free user id.'),
            'fdUidNumberBase', FALSE,
            0, FALSE, 1100
          ),
          new IntAttribute (
            _('Number base for groups'),
            _('Where to start looking for a new free group id.'),
            'fdGidNumberBase', FALSE,
            0, FALSE, 1100
          ),
          new StringAttribute (
            _('Users RDN'), _('The branch where user are stored.'),
            'fdUserRDN', TRUE,
            'ou=people'
          ),
          new StringAttribute (
            _('Groups RDN'), _('The branch where groups are stored.'),
            'fdGroupRDN', TRUE,
            'ou=groups'
          ),
        )
      ),
      'password' => array(
        'name'  => _('Password settings'),
        'attrs' => array(
          new SelectAttribute (
            _('Password default hash'), _('Default hash to be used'),
            'fdPasswordDefaultHash', TRUE,
            array('crypt/md5')
          ),
          new IntAttribute (
            _('Password minimum length'), _('Minimum length of user passwords'),
            'fdPasswordMinLength', FALSE,
            0 /*min*/, FALSE /*no max*/
          ),
          new IntAttribute (
            _('Password minimum differs'), _('Minimum number of different characters from last password'),
            'fdPasswordMinDiffer', FALSE,
            0 /*min*/, FALSE /*no max*/
          ),
          new StringAttribute (
            _('Password hook'), _('External script to handle password settings'),
            'fdPasswordHook'
          ),
          new BooleanAttribute (
            _('Use account expiration'),
            _('Enables shadow attribute tests during the login to FusionDirectory and forces password renewal or account lock‐out'),
            'fdHandleExpiredAccounts'
          ),
          new StringAttribute (
            _('SASL Realm'), _('SASL Realm'),
            'fdSaslRealm'
          ),
          new StringAttribute (
            _('SASL Exop'), _('Attribute to be stored in the userPassword attribute'),
            'fdSaslExop'
          ),
        )
      ),
      'core_settings' => array(
        'name'  => _('Core settings'),
        'attrs' => array(
          new BooleanAttribute (
            _('Enable primary group filter'),
            _('It is time consuming to evaluate which groups are primary and which are not, so you may want to disable it if your group plugin is slow.'),
            'fdPrimaryGroupFilter', FALSE,
            TRUE
          ),
          new BooleanAttribute (
            _('Display summary in listings'),
            _('Determines whether a status bar will be shown on the bottom of lists, displaying a short summary of type and number of elements in the list.'),
            'fdListSummary', FALSE,
            TRUE
          ),
          new BooleanAttribute (
            _('Honour administrative units'),
            _('Enables  checking  of unitTag attributes when using administrative units.'),
            'fdHonourUnitTags'
          ),
          new SelectAttribute (
            _('Edit locking'),
            _('Check if a entry currently being edited has been modified outside of FusionDirectory in the meantime.'),
            'fdModificationDetectionAttribute', FALSE,
            array('', 'entryCSN', 'contextCSN')
          ),
          new BooleanAttribute (
            _('Enable Copy & Paste'),
            _('Enables copy and paste for LDAP entries managed with FusionDirectory.'),
            'fdCopyPaste'
          ),
          new BooleanAttribute (
            _('Enable logging'),
            _('Event logging on FusionDirectory side.'),
            'fdLogging', FALSE,
            TRUE
          ),
        )
      ),
    );
  }

  function __construct(&$config, $dn = NULL, $object = NULL)
  {
    parent::__construct($config, $dn, $object);
    $this->attributesInfo['look_n_feel']['attrs']['fdTimezone']->setChoices(timezone::_get_tz_zones());
    $languages = get_languages(TRUE);
    asort($languages);
    $languages = array_merge(array("" => _("Automatic")), $languages);
    $this->attributesInfo['look_n_feel']['attrs']['fdLanguage']->setChoices(array_keys($languages), array_values($languages));
    $methods = passwordMethod::get_available_methods();
    $methods = $methods['name'];
    $this->attributesInfo['password']['attrs']['fdPasswordDefaultHash']->setChoices($methods);

  }

  static function get_themes()
  {
    $themesdir  = '../ihtml/themes/';
    $themes     = array();
    if ($dir = opendir("$themesdir")) {
      while (($file = readdir($dir)) !== FALSE) {
        if (is_dir("$themesdir/$file") && !preg_match("/^\./", $file)) {
          $themes[] = $file;
        }
      }
    }
    return $themes;
  }
}
?>