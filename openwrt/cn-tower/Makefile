# openwrt package for the game
# add the openwrt dir of this repo as a feed in your sdk
#   echo "src-link cntower /path/to/CN-Tower-C/openwrt" >> feeds.conf
#   ./scripts/feeds update cntower && ./scripts/feeds install cn-tower
#   make package/cn-tower/compile

include $(TOPDIR)/rules.mk

PKG_NAME:=cn-tower
PKG_VERSION:=1.0.0
PKG_RELEASE:=1

PKG_MAINTAINER:=Varyashine <varya@varyashine.ru>
PKG_LICENSE:=WTFPL

include $(INCLUDE_DIR)/package.mk

# sources come from the repo root or from a copy next to this makefile like ci does
CN_TOWER_ROOT:=$(if $(wildcard $(CURDIR)/src/main.c),$(CURDIR),$(CURDIR)/../..)

define Package/cn-tower
  SECTION:=utils
  CATEGORY:=Utilities
  SUBMENU:=Games
  TITLE:=text adventure about climbing the cn tower
  URL:=https://github.com/cherrywheel/CN-Tower-C
endef

define Package/cn-tower/description
  a text adventure about climbing torontos cn tower and doing the edgewalk
  with a full screen terminal ui and hints for what you can do right now
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) $(CN_TOWER_ROOT)/src/*.c $(CN_TOWER_ROOT)/include/*.h $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CPPFLAGS) $(TARGET_CFLAGS) -std=c99 -Wall -Wextra \
		-I$(PKG_BUILD_DIR) $(PKG_BUILD_DIR)/*.c -o $(PKG_BUILD_DIR)/cn-tower $(TARGET_LDFLAGS)
endef

define Package/cn-tower/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/cn-tower $(1)/usr/bin/
endef

$(eval $(call BuildPackage,cn-tower))
