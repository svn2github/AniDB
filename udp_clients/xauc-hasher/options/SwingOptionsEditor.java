/*
 * SwingOptionsEditor.java
 *
 * Created on 9/Abr/2009, 16:14:54
 */

package options;

import java.awt.event.ItemEvent;
import javax.swing.ImageIcon;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;

/**
 *
 * @author fahrenheit
 */
public class SwingOptionsEditor extends javax.swing.JFrame {
	private static final long serialVersionUID = 7704255408465416278L;
	protected String appDir = null;
	protected HasherOptions hasherOptions = null;
	protected RemoteOptions remoteOptions = null;
	protected boolean dirty = false;

    /**
     * Sets the Application Directory
	 * @param appDir The Application Directory to set
	 */
	public synchronized void setAppDir(String appDir) { this.appDir = appDir; }

	/**
	 * Sets the Hasher Options Object
	 * @param hasherOptions The Hasher Options object to use
	 */
	public synchronized void setHasherOptions(HasherOptions hasherOptions) { 
		this.hasherOptions = hasherOptions;
	}
	
	/**
	 * Sets the Remote Options Object
	 * @param hasherOptions The Remote Options object to use
	 */
	public synchronized void setRemoteOptions(RemoteOptions remoteOptions) { 
		this.remoteOptions = remoteOptions;
	}

	/** Creates new form SwingOptionsEditor */
    public SwingOptionsEditor() {
    	setLookAndFeel();
		this.setIconImage(new ImageIcon(SwingOptionsEditor.class.getResource("/icons/cog.png")).getImage());
        initComponents();
		loadSettings();
    }

	// Loads settings
	public synchronized void loadSettings() {
		if (hasherOptions != null) {
			this.hashing_enabled_ck.setSelected(hasherOptions.enabled);
			this.hashing_ed2k_ck.setSelected(hasherOptions.ed2k);
			this.hashing_crc32_ck.setSelected(hasherOptions.crc32);
			this.hashing_md5_ck.setSelected(hasherOptions.md5);
			this.hashing_sha1_ck.setSelected(hasherOptions.sha1);
			this.hashing_tth_ck.setSelected(hasherOptions.tth);
			// enable stuff
			this.hashing_ed2k_ck.setEnabled(hasherOptions.enabled);
			this.hashing_crc32_ck.setEnabled(hasherOptions.enabled);
			this.hashing_md5_ck.setEnabled(hasherOptions.enabled);
			this.hashing_sha1_ck.setEnabled(hasherOptions.enabled);
			this.hashing_tth_ck.setEnabled(hasherOptions.enabled);
		}
		if (remoteOptions != null) {
			this.remote_enabled_ck.setSelected(remoteOptions.enabled);
			this.remote_nat_ck.setSelected(remoteOptions.nat);
			this.remote_encryption_ck.setSelected(remoteOptions.encryption);
			this.remote_apihost_textfield.setText(remoteOptions.apihost);
			this.remote_apiport_textfield.setText(""+remoteOptions.apiport);
			this.remote_apikey_passfield.setText(remoteOptions.apipass);
			this.remote_protocol_combo.setSelectedIndex(remoteOptions.protocol);
			this.remote_localport_textfield.setText(""+remoteOptions.localport);
			this.remote_password_passfield.setText(remoteOptions.password);
			this.remote_username_textfield.setText(remoteOptions.username);
			// enable stuff
			this.remote_nat_ck.setEnabled(remoteOptions.enabled);
			this.remote_encryption_ck.setEnabled(remoteOptions.enabled);
			this.remote_apihost_textfield.setEnabled(remoteOptions.enabled);
			this.remote_apiport_textfield.setEnabled(remoteOptions.enabled);
			this.remote_apikey_passfield.setEnabled(remoteOptions.enabled);
			this.remote_protocol_combo.setEnabled(remoteOptions.enabled);
			this.remote_localport_textfield.setEnabled(remoteOptions.enabled);
			this.remote_password_passfield.setEnabled(remoteOptions.enabled);
			this.remote_username_textfield.setEnabled(remoteOptions.enabled);
			// enable labels
			this.remote_apihost_label.setEnabled(remoteOptions.enabled);
			this.remote_apikey_label.setEnabled(remoteOptions.enabled);
			this.remote_apiport_label.setEnabled(remoteOptions.enabled);
			this.remote_localport_label.setEnabled(remoteOptions.enabled);
			this.remote_misc_label.setEnabled(remoteOptions.enabled);
			this.remote_password_label.setEnabled(remoteOptions.enabled);
			this.remote_username_label.setEnabled(remoteOptions.enabled);
		}
	}

	// Save settings
	protected synchronized void saveSettings() {
		if (hasherOptions != null && appDir != null) {
			hasherOptions.enabled = this.hashing_enabled_ck.isSelected();
			hasherOptions.ed2k = this.hashing_ed2k_ck.isSelected();
			hasherOptions.crc32 = this.hashing_crc32_ck.isSelected();
			hasherOptions.md5 = this.hashing_md5_ck.isSelected();
			hasherOptions.sha1 = this.hashing_sha1_ck.isSelected();
			hasherOptions.tth = this.hashing_tth_ck.isSelected();
			hasherOptions.saveSettings(appDir);
		}
		if (remoteOptions != null && appDir != null) {
			remoteOptions.enabled = this.remote_enabled_ck.isSelected();
			remoteOptions.nat = this.remote_nat_ck.isSelected();
			remoteOptions.encryption = this.remote_encryption_ck.isSelected();
			remoteOptions.apihost = this.remote_apihost_textfield.getText();
			remoteOptions.apiport = new Integer(this.remote_apiport_textfield.getText());
			String pass = "";
			char[] charpass = this.remote_apikey_passfield.getPassword();
			for (int i = 0; i < charpass.length; i++)
				pass += charpass[i];
			java.util.Arrays.fill(charpass,'0');
			remoteOptions.apipass = pass;
			remoteOptions.protocol = new Integer(this.remote_protocol_combo.getSelectedIndex());
			remoteOptions.localport = new Integer(this.remote_localport_textfield.getText());
			pass = "";
			charpass = this.remote_password_passfield.getPassword();
			for (int i = 0; i < charpass.length; i++)
				pass += charpass[i];
			java.util.Arrays.fill(charpass,'0');
			remoteOptions.password = pass;
			remoteOptions.username = this.remote_username_textfield.getText();
			remoteOptions.saveSettings(appDir);
		}
	}

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        optionsPanels = new javax.swing.JTabbedPane();
        hashingPane = new javax.swing.JPanel();
        hashing_enabled_ck = new javax.swing.JCheckBox();
        hashing_algorithms = new javax.swing.JPanel();
        hashing_ed2k_ck = new javax.swing.JCheckBox();
        hashing_crc32_ck = new javax.swing.JCheckBox();
        hashing_md5_ck = new javax.swing.JCheckBox();
        hashing_sha1_ck = new javax.swing.JCheckBox();
        hashing_tth_ck = new javax.swing.JCheckBox();
        remotePane = new javax.swing.JPanel();
        remote_enabled_ck = new javax.swing.JCheckBox();
        remote_settings = new javax.swing.JPanel();
        remote_username_label = new javax.swing.JLabel();
        remote_username_textfield = new javax.swing.JTextField();
        remote_password_label = new javax.swing.JLabel();
        remote_password_passfield = new javax.swing.JPasswordField();
        remote_apihost_label = new javax.swing.JLabel();
        remote_apihost_textfield = new javax.swing.JTextField();
        remote_apiport_textfield = new javax.swing.JTextField();
        remote_protocol_combo = new javax.swing.JComboBox();
        remote_apikey_passfield = new javax.swing.JPasswordField();
        remote_apiport_label = new javax.swing.JLabel();
        remote_local_settings = new javax.swing.JPanel();
        remote_nat_ck = new javax.swing.JCheckBox();
        remote_encryption_ck = new javax.swing.JCheckBox();
        remote_localport_textfield = new javax.swing.JTextField();
        remote_localport_label = new javax.swing.JLabel();
        remote_misc_label = new javax.swing.JLabel();
        remote_apikey_label = new javax.swing.JLabel();
        apply_button = new javax.swing.JButton();
        cancel_button = new javax.swing.JButton();
        confirm_button = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("Preferences");
        setResizable(false);

        optionsPanels.setBackground(java.awt.SystemColor.controlLtHighlight);
        optionsPanels.setToolTipText("");

        hashingPane.setBackground(new java.awt.Color(255, 255, 255));

        hashing_enabled_ck.setBackground(java.awt.SystemColor.controlLtHighlight);
        hashing_enabled_ck.setText("Enabled");
        hashing_enabled_ck.setToolTipText("Check to enable hashing of files");
        hashing_enabled_ck.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                hashing_enabled_ckItemStateChanged(evt);
            }
        });

        hashing_algorithms.setBackground(java.awt.SystemColor.controlLtHighlight);
        hashing_algorithms.setBorder(javax.swing.BorderFactory.createTitledBorder("Algorithms"));

        hashing_ed2k_ck.setBackground(java.awt.SystemColor.controlLtHighlight);
        hashing_ed2k_ck.setText("Ed2k");
        hashing_ed2k_ck.setToolTipText("Enable hashing files using the Edonkey 2000 algorithm [fast]");
        hashing_ed2k_ck.setEnabled(false);
        hashing_ed2k_ck.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                hashing_ed2k_ckItemStateChanged(evt);
            }
        });

        hashing_crc32_ck.setBackground(java.awt.SystemColor.controlLtHighlight);
        hashing_crc32_ck.setText("CRC-32");
        hashing_crc32_ck.setToolTipText("Enable hashing files using the CRC-32 algorithm [very fast]");
        hashing_crc32_ck.setEnabled(false);
        hashing_crc32_ck.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                hashing_crc32_ckItemStateChanged(evt);
            }
        });

        hashing_md5_ck.setBackground(java.awt.SystemColor.controlLtHighlight);
        hashing_md5_ck.setText("MD5");
        hashing_md5_ck.setToolTipText("Enable hashing files using the MD5 algorithm [average]");
        hashing_md5_ck.setEnabled(false);
        hashing_md5_ck.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                hashing_md5_ckItemStateChanged(evt);
            }
        });

        hashing_sha1_ck.setBackground(java.awt.SystemColor.controlLtHighlight);
        hashing_sha1_ck.setText("SHA-1");
        hashing_sha1_ck.setToolTipText("Enable hashing files using the SHA-1 algorithm [slow]");
        hashing_sha1_ck.setEnabled(false);
        hashing_sha1_ck.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                hashing_sha1_ckItemStateChanged(evt);
            }
        });

        hashing_tth_ck.setBackground(java.awt.SystemColor.controlLtHighlight);
        hashing_tth_ck.setText("TTH");
        hashing_tth_ck.setToolTipText("Enable hashing files using the Tiger Tree Hash algorithm [very slow]");
        hashing_tth_ck.setEnabled(false);
        hashing_tth_ck.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                hashing_tth_ckItemStateChanged(evt);
            }
        });

        javax.swing.GroupLayout hashing_algorithmsLayout = new javax.swing.GroupLayout(hashing_algorithms);
        hashing_algorithms.setLayout(hashing_algorithmsLayout);
        hashing_algorithmsLayout.setHorizontalGroup(
            hashing_algorithmsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(hashing_algorithmsLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(hashing_algorithmsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(hashing_ed2k_ck)
                    .addComponent(hashing_crc32_ck)
                    .addComponent(hashing_md5_ck)
                    .addComponent(hashing_sha1_ck)
                    .addComponent(hashing_tth_ck))
                .addContainerGap(287, Short.MAX_VALUE))
        );
        hashing_algorithmsLayout.setVerticalGroup(
            hashing_algorithmsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(hashing_algorithmsLayout.createSequentialGroup()
                .addComponent(hashing_ed2k_ck)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(hashing_crc32_ck)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(hashing_md5_ck)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(hashing_sha1_ck)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(hashing_tth_ck)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        javax.swing.GroupLayout hashingPaneLayout = new javax.swing.GroupLayout(hashingPane);
        hashingPane.setLayout(hashingPaneLayout);
        hashingPaneLayout.setHorizontalGroup(
            hashingPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(hashingPaneLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(hashingPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(hashing_algorithms, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(hashing_enabled_ck))
                .addContainerGap())
        );
        hashingPaneLayout.setVerticalGroup(
            hashingPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(hashingPaneLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(hashing_enabled_ck)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(hashing_algorithms, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(35, Short.MAX_VALUE))
        );

        optionsPanels.addTab("Hashing", new javax.swing.ImageIcon(getClass().getResource("/icons/page_white_stack.png")), hashingPane); // NOI18N

        remotePane.setBackground(java.awt.SystemColor.controlLtHighlight);

        remote_enabled_ck.setBackground(java.awt.SystemColor.controlLtHighlight);
        remote_enabled_ck.setText("Enabled");
        remote_enabled_ck.setToolTipText("Check to enable access to the AniDB API");
        remote_enabled_ck.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                remote_enabled_ckItemStateChanged(evt);
            }
        });

        remote_settings.setBackground(java.awt.SystemColor.controlLtHighlight);
        remote_settings.setBorder(javax.swing.BorderFactory.createTitledBorder("Settings"));

        remote_username_label.setText("Username");
        remote_username_label.setEnabled(false);

        remote_username_textfield.setToolTipText("Your AniDB username as entered on the site");
        remote_username_textfield.setEnabled(false);
        remote_username_textfield.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyTyped(java.awt.event.KeyEvent evt) {
                remote_username_textfieldKeyTyped(evt);
            }
        });

        remote_password_label.setText("Password");
        remote_password_label.setEnabled(false);

        remote_password_passfield.setToolTipText("Your AniDB password as entered on the site");
        remote_password_passfield.setEnabled(false);
        remote_password_passfield.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyTyped(java.awt.event.KeyEvent evt) {
                remote_password_passfieldKeyTyped(evt);
            }
        });

        remote_apihost_label.setText("API host / port");
        remote_apihost_label.setEnabled(false);

        remote_apihost_textfield.setText("api.anidb.net");
        remote_apihost_textfield.setToolTipText("Remote API host name");
        remote_apihost_textfield.setEnabled(false);
        remote_apihost_textfield.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyTyped(java.awt.event.KeyEvent evt) {
                remote_apihost_textfieldKeyTyped(evt);
            }
        });

        remote_apiport_textfield.setText("9000");
        remote_apiport_textfield.setToolTipText("Remote API port");
        remote_apiport_textfield.setEnabled(false);
        remote_apiport_textfield.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyTyped(java.awt.event.KeyEvent evt) {
                remote_apiport_textfieldKeyTyped(evt);
            }
        });

        remote_protocol_combo.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "LOCAL", "UDP", "TCP" }));
        remote_protocol_combo.setToolTipText("Protocol to use");
        remote_protocol_combo.setEnabled(false);
        remote_protocol_combo.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                remote_protocol_comboItemStateChanged(evt);
            }
        });

        remote_apikey_passfield.setToolTipText("Your AniDB API Key as entered in your profile on the site");
        remote_apikey_passfield.setEnabled(false);
        remote_apikey_passfield.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyTyped(java.awt.event.KeyEvent evt) {
                remote_apikey_passfieldKeyTyped(evt);
            }
        });

        remote_apiport_label.setText(":");
        remote_apiport_label.setEnabled(false);

        remote_local_settings.setBackground(java.awt.SystemColor.controlLtHighlight);
        remote_local_settings.setBorder(javax.swing.BorderFactory.createTitledBorder("Local Options"));

        remote_nat_ck.setBackground(new java.awt.Color(255, 255, 255));
        remote_nat_ck.setText("NAT");
        remote_nat_ck.setToolTipText("Check this option to tell the server you are behind a NAT connection");
        remote_nat_ck.setEnabled(false);
        remote_nat_ck.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                remote_nat_ckItemStateChanged(evt);
            }
        });

        remote_encryption_ck.setBackground(java.awt.SystemColor.controlLtHighlight);
        remote_encryption_ck.setText("Encryption");
        remote_encryption_ck.setToolTipText("Use encryption to communicate with the server");
        remote_encryption_ck.setEnabled(false);
        remote_encryption_ck.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                remote_encryption_ckItemStateChanged(evt);
            }
        });

        remote_localport_textfield.setText("0");
        remote_localport_textfield.setToolTipText("Local Port to use, use 0 to indicate random port");
        remote_localport_textfield.setEnabled(false);
        remote_localport_textfield.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyTyped(java.awt.event.KeyEvent evt) {
                remote_localport_textfieldKeyTyped(evt);
            }
        });

        remote_localport_label.setText("Local port");
        remote_localport_label.setEnabled(false);

        remote_misc_label.setText("Misc");
        remote_misc_label.setEnabled(false);

        javax.swing.GroupLayout remote_local_settingsLayout = new javax.swing.GroupLayout(remote_local_settings);
        remote_local_settings.setLayout(remote_local_settingsLayout);
        remote_local_settingsLayout.setHorizontalGroup(
            remote_local_settingsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, remote_local_settingsLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(remote_misc_label)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(remote_nat_ck)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(remote_encryption_ck)
                .addGap(18, 18, 18)
                .addComponent(remote_localport_label)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(remote_localport_textfield, javax.swing.GroupLayout.PREFERRED_SIZE, 95, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(6, 6, 6))
        );
        remote_local_settingsLayout.setVerticalGroup(
            remote_local_settingsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(remote_local_settingsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                .addComponent(remote_localport_label)
                .addComponent(remote_localport_textfield, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addComponent(remote_encryption_ck)
                .addComponent(remote_misc_label)
                .addComponent(remote_nat_ck))
        );

        remote_apikey_label.setText("Key");
        remote_apikey_label.setEnabled(false);

        javax.swing.GroupLayout remote_settingsLayout = new javax.swing.GroupLayout(remote_settings);
        remote_settings.setLayout(remote_settingsLayout);
        remote_settingsLayout.setHorizontalGroup(
            remote_settingsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(remote_settingsLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(remote_settingsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(remote_local_settings, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(remote_settingsLayout.createSequentialGroup()
                        .addGroup(remote_settingsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(remote_apihost_label)
                            .addComponent(remote_username_label)
                            .addComponent(remote_password_label))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(remote_settingsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(remote_settingsLayout.createSequentialGroup()
                                .addComponent(remote_password_passfield, javax.swing.GroupLayout.PREFERRED_SIZE, 110, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(remote_apikey_label)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(remote_apikey_passfield, javax.swing.GroupLayout.DEFAULT_SIZE, 125, Short.MAX_VALUE))
                            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, remote_settingsLayout.createSequentialGroup()
                                .addComponent(remote_apihost_textfield, javax.swing.GroupLayout.DEFAULT_SIZE, 109, Short.MAX_VALUE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(remote_apiport_label)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(remote_apiport_textfield, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(remote_protocol_combo, javax.swing.GroupLayout.PREFERRED_SIZE, 96, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addComponent(remote_username_textfield, javax.swing.GroupLayout.DEFAULT_SIZE, 261, Short.MAX_VALUE))))
                .addContainerGap())
        );
        remote_settingsLayout.setVerticalGroup(
            remote_settingsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, remote_settingsLayout.createSequentialGroup()
                .addGroup(remote_settingsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(remote_apihost_label)
                    .addComponent(remote_apihost_textfield, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(remote_apiport_textfield, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(remote_protocol_combo, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(remote_apiport_label))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(remote_settingsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(remote_username_label)
                    .addComponent(remote_username_textfield, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(remote_settingsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(remote_settingsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                        .addComponent(remote_password_label)
                        .addComponent(remote_password_passfield, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addComponent(remote_apikey_label))
                    .addComponent(remote_apikey_passfield, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(9, 9, 9)
                .addComponent(remote_local_settings, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
        );

        javax.swing.GroupLayout remotePaneLayout = new javax.swing.GroupLayout(remotePane);
        remotePane.setLayout(remotePaneLayout);
        remotePaneLayout.setHorizontalGroup(
            remotePaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(remotePaneLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(remotePaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(remote_settings, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(remote_enabled_ck))
                .addContainerGap())
        );
        remotePaneLayout.setVerticalGroup(
            remotePaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(remotePaneLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(remote_enabled_ck)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(remote_settings, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(38, Short.MAX_VALUE))
        );

        optionsPanels.addTab("AniDB", new javax.swing.ImageIcon(getClass().getResource("/icons/anidb.png")), remotePane); // NOI18N

        apply_button.setText("Apply");
        apply_button.setToolTipText("Apply changes but don't close dialog");
        apply_button.setEnabled(false);
        apply_button.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                apply_buttonActionPerformed(evt);
            }
        });

        cancel_button.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/cancel.png"))); // NOI18N
        cancel_button.setText("Cancel");
        cancel_button.setToolTipText("Ignore changes and close dialog");
        cancel_button.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cancel_buttonActionPerformed(evt);
            }
        });

        confirm_button.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/accept.png"))); // NOI18N
        confirm_button.setText("OK");
        confirm_button.setToolTipText("Apply changes made and close dialog");
        confirm_button.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                confirm_buttonActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addContainerGap(160, Short.MAX_VALUE)
                .addComponent(confirm_button)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(cancel_button)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(apply_button)
                .addContainerGap())
            .addComponent(optionsPanels, javax.swing.GroupLayout.DEFAULT_SIZE, 393, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addComponent(optionsPanels, javax.swing.GroupLayout.PREFERRED_SIZE, 258, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(apply_button)
                    .addComponent(cancel_button)
                    .addComponent(confirm_button))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

	private void hashing_enabled_ckItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_hashing_enabled_ckItemStateChanged
		boolean isEnabled = (evt.getStateChange() == ItemEvent.SELECTED);
		this.hashing_ed2k_ck.setEnabled(isEnabled);
		this.hashing_crc32_ck.setEnabled(isEnabled);
		this.hashing_md5_ck.setEnabled(isEnabled);
		this.hashing_sha1_ck.setEnabled(isEnabled);
		this.hashing_tth_ck.setEnabled(isEnabled);
		enableApplyButton();
	}//GEN-LAST:event_hashing_enabled_ckItemStateChanged

	private void hashing_ed2k_ckItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_hashing_ed2k_ckItemStateChanged
		enableApplyButton();
	}//GEN-LAST:event_hashing_ed2k_ckItemStateChanged

	private void hashing_crc32_ckItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_hashing_crc32_ckItemStateChanged
		enableApplyButton();
	}//GEN-LAST:event_hashing_crc32_ckItemStateChanged

	private void hashing_md5_ckItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_hashing_md5_ckItemStateChanged
		enableApplyButton();
	}//GEN-LAST:event_hashing_md5_ckItemStateChanged

	private void hashing_sha1_ckItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_hashing_sha1_ckItemStateChanged
		enableApplyButton();
	}//GEN-LAST:event_hashing_sha1_ckItemStateChanged

	private void hashing_tth_ckItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_hashing_tth_ckItemStateChanged
		enableApplyButton();
	}//GEN-LAST:event_hashing_tth_ckItemStateChanged

	private void cancel_buttonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_cancel_buttonActionPerformed
		this.setVisible(false);
	}//GEN-LAST:event_cancel_buttonActionPerformed

	private void confirm_buttonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_confirm_buttonActionPerformed
		if (dirty) saveSettings();
		this.setVisible(false);
		this.dispose();
	}//GEN-LAST:event_confirm_buttonActionPerformed

	private void apply_buttonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_apply_buttonActionPerformed
		if (!dirty) return; // nothing to do
		saveSettings();
		dirty = false;
		apply_button.setEnabled(false); // disable the button again
	}//GEN-LAST:event_apply_buttonActionPerformed

	private void remote_enabled_ckItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_remote_enabled_ckItemStateChanged
		boolean isEnabled = (evt.getStateChange() == ItemEvent.SELECTED);
		// enable stuff
		this.remote_nat_ck.setEnabled(isEnabled);
		this.remote_encryption_ck.setEnabled(isEnabled);
		this.remote_apihost_textfield.setEnabled(isEnabled);
		this.remote_apiport_textfield.setEnabled(isEnabled);
		this.remote_apikey_passfield.setEnabled(isEnabled);
		this.remote_protocol_combo.setEnabled(isEnabled);
		this.remote_localport_textfield.setEnabled(isEnabled);
		this.remote_password_passfield.setEnabled(isEnabled);
		this.remote_username_textfield.setEnabled(isEnabled);
		// enable labels
		this.remote_apihost_label.setEnabled(isEnabled);
		this.remote_apikey_label.setEnabled(isEnabled);
		this.remote_apiport_label.setEnabled(isEnabled);
		this.remote_localport_label.setEnabled(isEnabled);
		this.remote_misc_label.setEnabled(isEnabled);
		this.remote_password_label.setEnabled(isEnabled);
		this.remote_username_label.setEnabled(isEnabled);
		enableApplyButton();
	}//GEN-LAST:event_remote_enabled_ckItemStateChanged

	private void remote_nat_ckItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_remote_nat_ckItemStateChanged
		enableApplyButton();
	}//GEN-LAST:event_remote_nat_ckItemStateChanged

	private void remote_encryption_ckItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_remote_encryption_ckItemStateChanged
		enableApplyButton();
	}//GEN-LAST:event_remote_encryption_ckItemStateChanged

	private void remote_localport_textfieldKeyTyped(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_remote_localport_textfieldKeyTyped
		enableApplyButton();
	}//GEN-LAST:event_remote_localport_textfieldKeyTyped

	private void remote_apikey_passfieldKeyTyped(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_remote_apikey_passfieldKeyTyped
		enableApplyButton();
	}//GEN-LAST:event_remote_apikey_passfieldKeyTyped

	private void remote_password_passfieldKeyTyped(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_remote_password_passfieldKeyTyped
		enableApplyButton();
	}//GEN-LAST:event_remote_password_passfieldKeyTyped

	private void remote_username_textfieldKeyTyped(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_remote_username_textfieldKeyTyped
		enableApplyButton();
	}//GEN-LAST:event_remote_username_textfieldKeyTyped

	private void remote_protocol_comboItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_remote_protocol_comboItemStateChanged
		enableApplyButton();
	}//GEN-LAST:event_remote_protocol_comboItemStateChanged

	private void remote_apiport_textfieldKeyTyped(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_remote_apiport_textfieldKeyTyped
		enableApplyButton();
	}//GEN-LAST:event_remote_apiport_textfieldKeyTyped

	private void remote_apihost_textfieldKeyTyped(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_remote_apihost_textfieldKeyTyped
		enableApplyButton();
	}//GEN-LAST:event_remote_apihost_textfieldKeyTyped

	public void enableApplyButton() {
		if (dirty) return; // no action needed
		dirty = true;
		this.apply_button.setEnabled(true);
	}

	public static void setLookAndFeel() {
		try {
			// Set platform Java L&F
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (UnsupportedLookAndFeelException e) {
			try { // revert to cross platform
				UIManager.setLookAndFeel(UIManager.getCrossPlatformLookAndFeelClassName());
			} catch (Exception ex) {
				System.err.println(ex.getLocalizedMessage());
				return;
			}
		} catch (Exception e) {
			System.err.println(e.getLocalizedMessage());
			return;
		}
	}
	
    /**
    * @param args the command line arguments
    */
    public static void main(String args[]) {
    	setLookAndFeel();
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new SwingOptionsEditor().setVisible(true);
            }
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton apply_button;
    private javax.swing.JButton cancel_button;
    private javax.swing.JButton confirm_button;
    private javax.swing.JPanel hashingPane;
    private javax.swing.JPanel hashing_algorithms;
    private javax.swing.JCheckBox hashing_crc32_ck;
    private javax.swing.JCheckBox hashing_ed2k_ck;
    private javax.swing.JCheckBox hashing_enabled_ck;
    private javax.swing.JCheckBox hashing_md5_ck;
    private javax.swing.JCheckBox hashing_sha1_ck;
    private javax.swing.JCheckBox hashing_tth_ck;
    private javax.swing.JTabbedPane optionsPanels;
    private javax.swing.JPanel remotePane;
    private javax.swing.JLabel remote_apihost_label;
    private javax.swing.JTextField remote_apihost_textfield;
    private javax.swing.JLabel remote_apikey_label;
    private javax.swing.JPasswordField remote_apikey_passfield;
    private javax.swing.JLabel remote_apiport_label;
    private javax.swing.JTextField remote_apiport_textfield;
    private javax.swing.JCheckBox remote_enabled_ck;
    private javax.swing.JCheckBox remote_encryption_ck;
    private javax.swing.JPanel remote_local_settings;
    private javax.swing.JLabel remote_localport_label;
    private javax.swing.JTextField remote_localport_textfield;
    private javax.swing.JLabel remote_misc_label;
    private javax.swing.JCheckBox remote_nat_ck;
    private javax.swing.JLabel remote_password_label;
    private javax.swing.JPasswordField remote_password_passfield;
    private javax.swing.JComboBox remote_protocol_combo;
    private javax.swing.JPanel remote_settings;
    private javax.swing.JLabel remote_username_label;
    private javax.swing.JTextField remote_username_textfield;
    // End of variables declaration//GEN-END:variables

}
