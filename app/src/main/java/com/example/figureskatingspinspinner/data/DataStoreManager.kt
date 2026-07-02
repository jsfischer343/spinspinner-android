package com.example.figureskatingspinspinner.data

import android.content.Context
import androidx.datastore.core.DataStore
import androidx.datastore.preferences.core.Preferences
import androidx.datastore.preferences.core.edit
import androidx.datastore.preferences.core.stringPreferencesKey
import androidx.datastore.preferences.preferencesDataStore
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.firstOrNull
import kotlinx.coroutines.flow.map
import kotlinx.coroutines.runBlocking

val Context.spinOptionsDataStore: DataStore<Preferences> by preferencesDataStore(name = "spin_options") //Adds the variable "spinOptionsDataStore" to the global context which then can be used anywhere in the app to reference the same DataStore

class DataStoreManager( //makes modifications to the provided dataStore in the constructor
    private val dataStore: DataStore<Preferences>
) {
    fun initDefaults(defaultsMap: Map<String, String>) {
        runBlocking {
            dataStore.updateData { preferences ->
                var updatedPreferences = preferences
                defaultsMap.forEach { (key, value) ->
                    if(preferences[stringPreferencesKey(key)] == null) {
                        updatedPreferences = updatedPreferences.toMutablePreferences().apply {
                            this[stringPreferencesKey(key)] = value
                        }.toPreferences()
                    }
                }
                updatedPreferences
            }
        }
    }
    suspend fun save(key: String, value: String) {
        dataStore.edit {
            it[stringPreferencesKey(key)] = value
        }
    }
    fun saveBlocking(key: String, value: String) {
        runBlocking {
            dataStore.edit {
                it[stringPreferencesKey(key)] = value
            }
        }
    }
    suspend fun get(key: String): String {
        return dataStore.data.map {
            it[stringPreferencesKey(key)] ?: ""
        }.firstOrNull() ?: ""
    }
    fun getBlocking(key: String): String {
        return runBlocking {
            dataStore.data.map {
                it[stringPreferencesKey(key)] ?: ""
            }.firstOrNull() ?: ""
        }
    }
    fun getFlowObj(key: String): Flow<String> {
        return dataStore.data.map {
            it[stringPreferencesKey(key)] ?: ""
        }
    }
    suspend fun clearAll() {
        dataStore.edit {
            it.clear()
        }
    }
}