package com.test.pptcontrol;
/***
 * ����һ����Ϣ���ý���
 */

import android.content.Intent;
import android.os.Bundle;
import android.preference.EditTextPreference;
import android.preference.ListPreference;
import android.preference.Preference;
import android.preference.Preference.OnPreferenceChangeListener;
import android.preference.PreferenceActivity;
import android.preference.PreferenceManager;

public class SpeechControl extends PreferenceActivity implements
OnPreferenceChangeListener{
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		//PreferenceManager��ʼ��
		PreferenceManager preferenceManager = getPreferenceManager();
		preferenceManager.setSharedPreferencesName(getPackageName());
		preferenceManager.setSharedPreferencesMode(MODE_PRIVATE);
		addPreferencesFromResource(R.xml.preference_iat);
		
		//ip��ַ����
		EditTextPreference ipPreference = (EditTextPreference)
				findPreference(getString(R.string
				.set_ip));
			ipPreference.setOnPreferenceChangeListener(this);
			ipPreference.setSummary(ipPreference.getText());
		//
		//�˿ڵ�ַ����
		EditTextPreference portPreference = (EditTextPreference)
				findPreference(getString(R.string
				.set_port));
			portPreference.setOnPreferenceChangeListener(this);
			portPreference.setSummary(portPreference.getText());
		//�����ʲ����б�.
		ListPreference rateListPreference = (ListPreference)
			findPreference(getString(R.string
			.preference_key_iat_rate));
		rateListPreference.setOnPreferenceChangeListener(this);
		rateListPreference.setSummary(rateListPreference.getEntry());
		//
	}

	/**
	 * OnPreferenceChangeListener�ص��ӿڣ������ò����б�
	 * ѡ��ֵ�޸�֮�󱻵���.
	 * @param preference
	 * @param newValue
	 */
	@Override
	public boolean onPreferenceChange(Preference preference, Object newValue) {
		if (preference instanceof ListPreference) {
			//��ȡ��ǰ�ĵ����б�UI����
			ListPreference listPreference = (ListPreference) preference;
			//
			CharSequence[] entries = listPreference.getEntries();
			//��ȡ��ǰ��������б�ĵڼ���Ԫ��.
			int index = listPreference.findIndexOfValue((String) newValue);

			listPreference.setSummary(entries[index]);
		}
		//��ʾip�ı��ֵ
		if(preference.getKey().equals("setIp"))
		{
			EditTextPreference ipPreference = (EditTextPreference)preference;
			ipPreference.setSummary(ipPreference.getText());
		}
		//��ʾport�ı��ֵ
		if(preference.getKey().equals("setPort"))
		{
			EditTextPreference portPreference = (EditTextPreference)preference;
			portPreference.setSummary(portPreference.getText());
		}
		refresh();
		/*
		if(preference instanceof EditTextPreference)
		{
			EditTextPreference ipPreference = (EditTextPreference)preference;
			ipPreference.setSummary(ipPreference.getText());
		}
		*/
		return true;
	}
	//
	public void finish() {
		super.finish();
	}
	//ҳ��ˢ��
	private void refresh() {
        finish();
        Intent intent = new Intent(SpeechControl.this, SpeechControl.class);
        startActivity(intent);
    }
}
