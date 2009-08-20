using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace DvdKhl.Controls {
    sealed public class ListViewExt : ListView {
        private IVirtualMode virtualMode;
        private int firstCacheItemIndex;
        private ListViewItem[] cachedItems;

        public ListViewExt() {
            OwnerDraw = true;
            DoubleBuffered = true;
        }

        public new ListViewExtItemCollection Items { get { return (ListViewExtItemCollection)base.Items; } }

        public void SetVirtualMode(IVirtualMode virtualMode) {
            this.virtualMode = virtualMode;
            RetrieveVirtualItem += new RetrieveVirtualItemEventHandler(GetItem);
            CacheVirtualItems += new CacheVirtualItemsEventHandler(Cache);
            VirtualMode = true;
        }
        private void Cache(object sender, CacheVirtualItemsEventArgs e) {
            if(cachedItems != null && e.StartIndex >= firstCacheItemIndex && e.EndIndex <= firstCacheItemIndex + cachedItems.Length) return;
            int length = e.EndIndex - e.StartIndex;
            firstCacheItemIndex = e.StartIndex;
            cachedItems = new ListViewItem[length];

            for(int i = 0;i < length;i++) cachedItems[i] = virtualMode[i];
        }
        private void GetItem(object sender, RetrieveVirtualItemEventArgs e) {
            if(false & cachedItems != null && e.ItemIndex >= firstCacheItemIndex && e.ItemIndex < firstCacheItemIndex + cachedItems.Length) {
                e.Item = cachedItems[e.ItemIndex - firstCacheItemIndex];
            } else {
                e.Item = virtualMode[e.ItemIndex];
            }
        }

        public class ListViewExtItemCollection : ListViewItemCollection {
            public ListViewExtItemCollection(ListViewExt owner)
                : base(owner) {

            }
        }
    }

    public class ListViewExtItem : ListViewItem {
        private Control control;

        public Control Control { get { return control; } set { control = value; } }

        public ListViewExtItem() {
        }

        public new ListViewExtSubItemCollection SubItems { get { return (ListViewExtSubItemCollection)SubItems; } }

        public class ListViewExtSubItem : ListViewSubItem {
            private Control control;

            public Control Control { get { return control; } set { control = value; } }
        }
        public class ListViewExtSubItemCollection : ListViewSubItemCollection {
            public ListViewExtSubItemCollection(ListViewExtItem owner)
                : base(owner) {

            }

            public override ListViewSubItem this[string key] {
                get {
                    return base[key];
                }
            }

        }
    }

    public interface IVirtualMode {
        ListViewExtItem this[int index] { get; }
    }

}
