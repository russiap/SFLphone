<?php
$name = 'download';
$javascript = array('jquery','animated-menu');

include_once('utils.php');

include('include/header.php');

?>
    <div id="content">
      <h1 class="title alignLeft fntBleu" style="margin:10px; padding:10px;">Binaries download</h1>
      <h4 class= "alignLeft fntBleu" style="margin:10px; padding:10px;">
      Latest version: <? echo $GLOBALS['SFLPHONE_LATEST_VERSION']; ?>
      <br>Release Date: 
      <? echo $GLOBALS['SFLPHONE_RELEASE_DATE']; ?>
      </h4>
      <table class="download-table" >
        <thead>
          <th>System</th>
          <th>Notes</th>
          <th>Maintainer</th>
        </thead>
        <tbody>
          <tr>
            <td><img class="logo" src="img/logos/small/ubuntu.png" alt="Ubuntu downloads" /></td>
            <td>
              <ul style="margin:10px; padding:5px;">
                <li>Ubuntu 8.10 "Intrepid Ibex" <a href="http://www.sflphone.org/debian/dists/intrepid/universe/binary-i386/sflphone_0.9.3-0ubuntu1_i386.deb">i386</a> | <a href="http://www.sflphone.org/debian/dists/intrepid/universe/binary-amd64/sflphone_0.9.3-0ubuntu1_amd64.deb" >amd64</a>
              </li>
              <li>
              Ubuntu 8.04 LTS "Hardy Heron" <a href="http://www.sflphone.org/debian/dists/hardy/universe/binary-i386/sflphone_0.9.2-0ubuntu7_i386.deb" >i386</a> | <a href="http://www.sflphone.org/debian/dists/hardy/universe/binary-amd64/sflphone_0.9.2-0ubuntu7_amd64.deb" >amd64</a></li>
              </ul>
              <br/>
              <a href="http://dev.savoirfairelinux.net/sflphone/wiki/DebianPackaging">add our repository</a>
               | <a href="">release note</a>  
               | <a href="">changelog</a>
            </td>
            <td><a href="mailto:emmanuel.milou@savoirfairelinux.com">Emmanuel Milou</a></td>
          </tr>
          <tr>
            <td><img class="logo" src="img/logos/small/fedora.png" alt="Fedora downloads" /></td>
            <td>
            <ul style="margin:10px; padding:5px;">
              <li>
              Fedora 9 "Sulphur" <a href="http://www.sflphone.org/downloads/sflphone-0.9.2-2.i386.rpm" >i386</a><br />
              </li>
            </ul>
              <a href="http://dev.savoirfairelinux.net/sflphone/wiki/YumRepository">add our repository</a> 
              | <a href="">release note</a>
              | <a href="">changelog</a>
            </td>
            <td><a href="mailto:emmanuel.milou@savoirfairelinux.com">Emmanuel Milou</a></td>
          </tr>
          <tr>
            <td><img  class="logo"src="img/logos/small/suse.png" alt="Suse downloads" /></td>
            <td>Follow the steps to add our Suse yum repository <a href="http://dev.savoirfairelinux.net/sflphone/wiki/DownloadSFLphone#Binaries">here</a>.</td>
            <td><a href="mailto:emmanuel.milou@savoirfairelinux.com">Emmanuel Milou</a></td>
          </tr>
            <tr>
            <td><img class="logo" src="img/logos/small/debian.png" alt="Debian downloads" /></td>
            <td>
              <i>Soon available</i>  </td>
            <td></td>
          </tr>

        </tbody>
      </table>
      <h1 class="title alignLeft fntBleu"  style="margin:10px; padding:10px;">Sources download</h1>
      <table class="download-table" >
        <thead>
          <th>Sources</th> 
          <th>Notes</th>
        </thead>
        <tbody>
          <tr>
            <td><a href="http://dev.savoirfairelinux.net/sflphone/downloads/5">sflphone_0.9.2.orig.tar.gz</a></td>
            <td>Compressed code source</td>
          </tr>
          <tr>
            <td><a href="http://dev.savoirfairelinux.net/sflphone/browser">GIT Trac browser</a></td>
            <td>Fetch the latest code with git: <br/><i>git clone http://sflphone.org/git/sflphone.git</i></td>
          </tr>
          <tr>
            <td>apt-get</td>
            <td><i>apt-get source sflphone</i></td>
          </tr>
        </tbody>
      </table>
    </div>

<?php

include('include/footer.php');