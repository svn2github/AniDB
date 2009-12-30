package aniAdd.misc;

import java.util.ArrayList;
import java.util.Collection;
import java.util.TreeMap;

public class MultiKeyDict<C, K, V> {

    private IKeyMapper<C, K, V> keyMapper;
    private ArrayList<TreeMap<K, V>> dict;

    public MultiKeyDict(IKeyMapper<C, K, V> keyMapper) {
        this.keyMapper = keyMapper;
        dict = new ArrayList<TreeMap<K, V>>();

        for (int i = 0; i < keyMapper.count(); i++) {
            dict.add(new TreeMap<K, V>());
        }
    }

    public void clear() {
        for (int i = 0; i < keyMapper.count(); i++) {
            dict.get(i).clear();
        }
    }

    public V get(C cat, K key) {
        return dict.get(keyMapper.getCatIndex(cat)).get(key);
    }

    public void put(V value) {
        for (int i = 0; i < keyMapper.count(); i++) {
            dict.get(i).put(keyMapper.getKey(i, value), value);
        }
    }

    public void remove(C cat, K key) {
        dict.get(keyMapper.getCatIndex(cat)).remove(key);
    }

    public boolean contains(C cat, K key) {
        return dict.get(keyMapper.getCatIndex(cat)).containsKey(key);
    }

    public Collection<V> values() {
        return dict.size() != 0 ? dict.get(0).values() : new ArrayList<V>();
    }

    public int size() {
        return dict.size() != 0 ? dict.get(0).size() : 0;
    }

    public interface IKeyMapper<C, K, V> {

        int count();

        int getCatIndex(C category);

        K getKey(int index, V value);
    }
}
